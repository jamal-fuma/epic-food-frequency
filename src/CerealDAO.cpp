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

#include "dao/CerealDAO.hpp"
#include "dao/Cereal.hpp"
#include "dao/Food.hpp"
#include "dao/Weight.hpp"
#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

// find a cereal given an id
bool Epic::CerealDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Cereal & cereal)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        cereal.set_id(id);
        cereal.set_food_id(m_find_by_id.column_int64(0));
        cereal.set_weight_id(m_find_by_id.column_int64(1));
        rc = cereal.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a cereal given a food_id
bool Epic::CerealDAO::DataAccess::find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Cereal & cereal)
{
    bool rc = false;
    m_find_by_food_id.bind_int64(1,food_id);
    if(SQLITE_ROW == m_find_by_food_id.step())
    {
        cereal.set_id(m_find_by_food_id.column_int64(0));
        cereal.set_food_id(food_id);
        cereal.set_weight_id(m_find_by_food_id.column_int64(1));
        rc = cereal.validate();
    }
    m_find_by_food_id.reset();
    return rc;
}

// save a cereal
bool Epic::CerealDAO::DataAccess::save(Epic::DAO::Cereal & cereal)
{
    m_insert.bind_int64(1,cereal.get_food_id());
    m_insert.bind_int64(2,cereal.get_weight_id());

    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        cereal.set_id(Epic::Database::last_insert_id());
        return cereal.validate();
    }
    return false;
}

// singleton versions

// find a cereal given an id
bool Epic::CerealDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::CerealDAO::DataAccess >::instance().find_by_id(id,cereal);
}

// find a cereal given a name
bool Epic::CerealDAO::find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::CerealDAO::DataAccess >::instance().find_by_food_id(food_id,cereal);
}

// save a cereal
bool Epic::CerealDAO::save(Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::CerealDAO::DataAccess >::instance().save(cereal);
}

// wire up saving the model to the DAO
bool Epic::DAO::Cereal::save()
{   
    return Epic::CerealDAO::save(*this);
}

// wire up finding the model using the DAO and a name
Epic::DAO::Cereal Epic::DAO::Cereal::find_by_food_id(sqlite3_int64 food_id)
{
    Epic::DAO::Cereal cereal;
    Epic::CerealDAO::find_by_food_id(food_id,cereal);
    if(!cereal.valid())
        Epic::Logging::Error().log() << "invalid cereal with food_id of: " << food_id; 
    return cereal;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Cereal Epic::DAO::Cereal::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Cereal cereal;
    Epic::CerealDAO::find_by_id(id,cereal);
    return cereal;
}

// load the model associations from file
bool Epic::DAO::Cereal::load()
{
    std::string value;
    std::string config_key = "cereals";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Cereal::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

// load the model from file
bool Epic::DAO::Cereal::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Cereal cereal;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("FOOD_CODE");
                expected.push_back("WEIGHT_ID");
                 if(Epic::Import::DBModel::same_header("cereals",expected,v))
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
            if(cnf.find("FOOD_CODE",s))
            {
                cereal.set_food_id(Epic::DAO::Food::find_by_name(s).get_id());
            }
            if(cnf.find("WEIGHT_ID",s))
            {
                cereal.set_weight_id(Epic::DAO::Weight::find_by_id(Epic::Conversion::IntegerString(s)).get_id());
            }

            if(!cereal.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "cereals" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

