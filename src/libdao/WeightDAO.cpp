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

#include "WeightDAO.hpp"
#include "dao/Weight.hpp"
#include "conversion/Conversion.hpp"
#include "import/Import.hpp"
#include "libcsv/MatchedValues.hpp"
#include "libcsv/CSVReader.hpp"

// find a weight given an id
bool Epic::WeightDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Weight & weight)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        weight.set_id(id);
        weight.set_amount(m_find_by_id.column_double(0));
        rc = weight.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find the upper and lower bounds of the weight range
bool Epic::WeightDAO::DataAccess::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
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

// find all weights
bool Epic::WeightDAO::DataAccess::find_all(std::vector<Epic::DAO::Weight> & weights)
{
    bool rc = false;
    Epic::DAO::Weight weight;
    rc = (SQLITE_ROW == m_find_all.step());
    if(!rc)
        return false;

    while(rc)
    {
        weight.set_id(m_find_all.column_int64(0));
        weight.set_amount(m_find_all.column_double(1));
        weight.validate();
        weights.push_back(weight);
        rc = (SQLITE_ROW == m_find_all.step());
    }
    m_find_all.reset();
    return true;
}


// save a weight
bool Epic::WeightDAO::DataAccess::save(Epic::DAO::Weight & weight)
{
    m_insert.bind_double(1,weight.get_amount());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        weight.set_id(Epic::Database::last_insert_id());
        return weight.validate();
    }
    return false;
}

// singleton versions

// find a weight given an id
bool Epic::WeightDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Weight & weight)
{
    return Epic::Pattern::Singleton< Epic::WeightDAO::DataAccess >::instance().find_by_id(id,weight);
}

// find the upper and lower bounds of the weight range
bool Epic::WeightDAO::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    return Epic::Pattern::Singleton< Epic::WeightDAO::DataAccess >::instance().find_bounds(upper,lower);
}

// find all weights
bool Epic::WeightDAO::find_all(std::vector<Epic::DAO::Weight> & weights)
{
    return Epic::Pattern::Singleton< Epic::WeightDAO::DataAccess >::instance().find_all(weights);
}

// save a weight
bool Epic::WeightDAO::save(Epic::DAO::Weight & weight)
{
    return Epic::Pattern::Singleton< Epic::WeightDAO::DataAccess >::instance().save(weight);
}

// wire up saving the model to the DAO
bool Epic::DAO::Weight::save()
{
    return Epic::WeightDAO::save(*this);
}

// wire up finding all weights
bool Epic::DAO::Weight::find_all(std::vector<Epic::DAO::Weight> & weights)
{
    return Epic::WeightDAO::find_all(weights);
}

// wire up finding the model using the DAO and an id
Epic::DAO::Weight Epic::DAO::Weight::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Weight weight;
    Epic::WeightDAO::find_by_id(id,weight);
    return weight;
}

// wire up finding the upper and lower bounds of the weight range to the model
bool Epic::DAO::Weight::find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower)
{
    return Epic::WeightDAO::find_bounds(upper,lower);
}

// load the model associations from file
bool Epic::DAO::Weight::load()
{
    std::string value;
    std::string config_key = "weights";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Weight::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}


// load the model from file
bool Epic::DAO::Weight::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Weight weight;

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
                if(Epic::Import::MatchedValues()("weights",expected,v))
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
                weight.set_amount( Epic::Conversion::DecimalString(s));
            }

            if(!weight.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "weights" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}
