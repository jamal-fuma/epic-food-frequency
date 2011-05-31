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

#include "FrequencyDAO.hpp"
#include "dao/Frequency.hpp"
#include "conversion/Conversion.hpp"
#include "import/Import.hpp"

// find a frequency given an id
bool Epic::FrequencyDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Frequency & frequency)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        frequency.set_id(id);
        frequency.set_amount(m_find_by_id.column_double(0));
        rc = frequency.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find the upper and lower bounds of the frequency range
bool Epic::FrequencyDAO::DataAccess::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
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

// find all frequencies
bool Epic::FrequencyDAO::DataAccess::find_all(std::vector<Epic::DAO::Frequency> & frequencies)
{
    bool rc = false;
    Epic::DAO::Frequency frequency;
    rc = (SQLITE_ROW == m_find_all.step());
    if(!rc)
        return false;

    while(rc)
    {
        frequency.set_id(m_find_all.column_int64(0));
        frequency.set_amount(m_find_all.column_double(1));
        frequency.validate();
        frequencies.push_back(frequency);
        rc = (SQLITE_ROW == m_find_all.step());
    }
    m_find_all.reset();
    return true;
}


// save a frequency
bool Epic::FrequencyDAO::DataAccess::save(Epic::DAO::Frequency & frequency)
{
    m_insert.bind_double(1,frequency.get_amount());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        frequency.set_id(Epic::Database::last_insert_id());
        return frequency.validate();
    }
    return false;
}

// singleton versions

// find a frequency given an id
bool Epic::FrequencyDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Frequency & frequency)
{
    return Epic::Pattern::Singleton< Epic::FrequencyDAO::DataAccess >::instance().find_by_id(id,frequency);
}

// save a frequency
bool Epic::FrequencyDAO::save(Epic::DAO::Frequency & frequency)
{
    return Epic::Pattern::Singleton< Epic::FrequencyDAO::DataAccess >::instance().save(frequency);
}

// find the upper and lower bounds of the frequency range
bool Epic::FrequencyDAO::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    return Epic::Pattern::Singleton< Epic::FrequencyDAO::DataAccess >::instance().find_bounds(upper,lower);
}

// find all frequencies
bool Epic::FrequencyDAO::find_all(std::vector<Epic::DAO::Frequency> & frequencies)
{
    return Epic::Pattern::Singleton< Epic::FrequencyDAO::DataAccess >::instance().find_all(frequencies);
}

// wire up saving the model to the DAO
bool Epic::DAO::Frequency::save()
{
    return Epic::FrequencyDAO::save(*this);
}

// wire up finding the model using the DAO and an id
Epic::DAO::Frequency Epic::DAO::Frequency::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Frequency frequency;
    Epic::FrequencyDAO::find_by_id(id,frequency);
    return frequency;
}

// find all frequencies
bool Epic::DAO::Frequency::find_all(std::vector<Epic::DAO::Frequency> & frequencies)
{
    return Epic::FrequencyDAO::find_all(frequencies);
}

// wire up finding the upper and lower bounds of the frequency range to the model
bool Epic::DAO::Frequency::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    return Epic::FrequencyDAO::find_bounds(upper,lower);
}

// load the model associations from file
bool Epic::DAO::Frequency::load()
{
    std::string value;
    std::string config_key = "frequencies";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Frequency::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

// load the model from file
bool Epic::DAO::Frequency::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Frequency frequency;

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
                if(Epic::Import::DBModel::same_header("frequencies",expected,v))
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
                frequency.set_amount( Epic::Conversion::DecimalString(s));
            }

            if(!frequency.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "frequencies" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}
