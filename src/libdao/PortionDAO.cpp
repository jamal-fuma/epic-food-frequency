/* This file is part of the libEpic client */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "PortionDAO.hpp"
#include "dao/Portion.hpp"
#include "conversion/Conversion.hpp"
#include "import/Import.hpp"
#include "libcsv/MatchedValues.hpp"
#include "libcsv/CSVReader.hpp"

// find a portion given an id
bool Epic::PortionDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        portion.set_id(id);
        portion.set_amount(m_find_by_id.column_double(0));
        rc = portion.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find the upper and lower bounds of the portion range
bool Epic::PortionDAO::DataAccess::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    bool rc = false;
    if(SQLITE_ROW == m_find_bounds.step())
    {
        lower = m_find_bounds.column_int64(0);
        upper = m_find_bounds.column_int64(1);
        rc = true;
    }
    m_find_bounds.reset();
    return rc;
}

// find all portions
bool Epic::PortionDAO::DataAccess::find_all(std::vector<Epic::DAO::Portion> & portions)
{
    bool rc = false;
    Epic::DAO::Portion portion;
    rc = (SQLITE_ROW == m_find_all.step());
    if(!rc)
        return false;

    while(rc)
    {
        portion.set_id(m_find_all.column_int64(0));
        portion.set_amount(m_find_all.column_double(1));
        portion.validate();
        portions.push_back(portion);
        rc = (SQLITE_ROW == m_find_all.step());
    }
    m_find_all.reset();
    return true;
}



// save a portion
bool Epic::PortionDAO::DataAccess::save(Epic::DAO::Portion & portion)
{
    m_insert.bind_double(1,portion.get_amount());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        portion.set_id(Epic::Database::last_insert_id());
        return portion.validate();
    }
    return false;
}

// singleton versions

// find a portion given an id
bool Epic::PortionDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion)
{
    return Epic::Pattern::Singleton< Epic::PortionDAO::DataAccess >::instance().find_by_id(id,portion);
}

// find the upper and lower bounds of the portion range
bool Epic::PortionDAO::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    return Epic::Pattern::Singleton< Epic::PortionDAO::DataAccess >::instance().find_bounds(upper,lower);
}

// find all portions
bool Epic::PortionDAO::find_all(std::vector<Epic::DAO::Portion> & portions)
{
    return Epic::Pattern::Singleton< Epic::PortionDAO::DataAccess >::instance().find_all(portions);
}


// save a portion
bool Epic::PortionDAO::save(Epic::DAO::Portion & portion)
{
    return Epic::Pattern::Singleton< Epic::PortionDAO::DataAccess >::instance().save(portion);
}

// wire up saving the model to the DAO
bool Epic::DAO::Portion::save()
{
    return Epic::PortionDAO::save(*this);
}

// wire up finding all portions
bool Epic::DAO::Portion::find_all(std::vector<Epic::DAO::Portion> & portions)
{
    return Epic::PortionDAO::find_all(portions);
}

// wire up finding the upper and lower bounds of the portion range to the model
bool Epic::DAO::Portion::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    return Epic::PortionDAO::find_bounds(upper,lower);
}



// wire up finding the model using the DAO and an id
Epic::DAO::Portion Epic::DAO::Portion::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Portion portion;
    Epic::PortionDAO::find_by_id(id,portion);
    return portion;
}

// load the model associations from file
bool Epic::DAO::Portion::load()
{
    std::string value;
    std::string config_key = "portions";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Portion::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

// load the model from file
bool Epic::DAO::Portion::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Portion portion;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("CODE");
                expected.push_back("VALUE");
                if(Epic::Import::MatchedValues()("portions",expected,v))
                {
                    h.swap(v);
                    continue;
                }
                return false;
            }

            Epic::Import::str_vector_t::size_type end = v.size();
            for(Epic::Import::str_vector_t::size_type pos=0; pos != end; ++pos)
            {
                cnf.insert(h[pos],v[pos],true);
            }

            std::string s;
            if(cnf.find("VALUE",s))
            {
                portion.set_amount( Epic::Conversion::DecimalString(s));
            }

            if(!portion.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "portions" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}


