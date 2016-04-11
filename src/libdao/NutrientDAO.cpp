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

#include "NutrientDAO.hpp"
#include "dao/Nutrient.hpp"

#include "conversion/Conversion.hpp"
#include "libcsv/MatchedValues.hpp"
#include "libcsv/CSVReader.hpp"
#include "import/Import.hpp"

// find a nutrient given an id
bool Epic::NutrientDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Nutrient & nutrient)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        nutrient.set_id(id);
        nutrient.set_code(m_find_by_id.column_int(0));
        nutrient.set_description(m_find_by_id.column_text(1));
        nutrient.set_units(m_find_by_id.column_text(2));
        rc = nutrient.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a nutrient given a code
bool Epic::NutrientDAO::DataAccess::find_by_code(int code, Epic::DAO::Nutrient & nutrient)
{
    bool rc = false;
    m_find_by_code.bind_int(1,code);
    if(SQLITE_ROW == m_find_by_code.step())
    {
        nutrient.set_id(m_find_by_code.column_int64(0));
        nutrient.set_code(code);
        nutrient.set_description(m_find_by_code.column_text(1));
        nutrient.set_units(m_find_by_code.column_text(2));

        rc = nutrient.validate();
    }
    m_find_by_code.reset();
    return rc;
}

// find all nutrients
bool Epic::NutrientDAO::DataAccess::find_all(std::vector<Epic::DAO::Nutrient> & nutrients)
{
    bool rc = false;
    std::vector<Epic::DAO::Nutrient> tmp;
    tmp.swap(nutrients);
    Epic::DAO::Nutrient nutrient;
    rc = (SQLITE_ROW == m_find_all.step());
    if(!rc)
        return false;

    while(rc)
    {
        nutrient.set_id(m_find_all.column_int64(0));
        nutrient.set_code(m_find_all.column_int(1));
        nutrient.set_description(m_find_all.column_text(2));
        nutrient.set_units(m_find_all.column_text(3));
        nutrient.validate();
        nutrients.push_back(nutrient);
        rc = (SQLITE_ROW == m_find_all.step());
    }
    m_find_all.reset();
    return true;
}



// save a nutrient
bool Epic::NutrientDAO::DataAccess::save(Epic::DAO::Nutrient & nutrient)
{
    if(nutrient.description_empty() || nutrient.units_empty())
    {
        Epic::Logging::Error().log() << "Cant save a nutrient without a description and units";
        return false;
    }

    int code = nutrient.get_code();
    if(code <= 0 )
    {
        Epic::Logging::Error().log() << "Cant save a nutrient without a code greater then zero";
        return false;
    }

    // check if it exists already
    if(!find_by_code(code,nutrient))
    {
        m_insert.bind_int(1,code);
	m_insert.bind_text(2,nutrient.get_description());
	m_insert.bind_text(3,nutrient.get_units());

        int rc = m_insert.step();
        m_insert.reset();
        if(rc == SQLITE_DONE)
        {
            nutrient.set_id(Epic::Database::last_insert_id());
            return nutrient.validate();
        }
    }

    // no-op as allready present in table
    return true;
}

// singleton versions

// find a nutrient given an id
bool Epic::NutrientDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Nutrient & nutrient)
{
    return Epic::Pattern::Singleton< Epic::NutrientDAO::DataAccess >::instance().find_by_id(id,nutrient);
}

// find all nutrients
bool Epic::NutrientDAO::find_all(std::vector<Epic::DAO::Nutrient> & nutrients)
{
    return Epic::Pattern::Singleton< Epic::NutrientDAO::DataAccess >::instance().find_all(nutrients);
}

// find a nutrient given a code
bool Epic::NutrientDAO::find_by_code(int code, Epic::DAO::Nutrient & nutrient)
{
    return Epic::Pattern::Singleton< Epic::NutrientDAO::DataAccess >::instance().find_by_code(code,nutrient);
}

// save a nutrient
bool Epic::NutrientDAO::save(Epic::DAO::Nutrient & nutrient)
{
    return Epic::Pattern::Singleton< Epic::NutrientDAO::DataAccess >::instance().save(nutrient);
}

// wire up saving the model to the DAO
bool Epic::DAO::Nutrient::save()
{
    return Epic::NutrientDAO::save(*this);
}

// wire up finding the model using the DAO and a code
Epic::DAO::Nutrient Epic::DAO::Nutrient::find_by_code(int code)
{
    Epic::DAO::Nutrient nutrient;
    Epic::NutrientDAO::find_by_code(code,nutrient);
    return nutrient;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Nutrient Epic::DAO::Nutrient::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Nutrient nutrient;
    Epic::NutrientDAO::find_by_id(id,nutrient);
    return nutrient;
}

// wire up finding all nutrients
bool Epic::DAO::Nutrient::find_all(std::vector<Epic::DAO::Nutrient> & nutrients)
{
    return Epic::NutrientDAO::find_all(nutrients);
}

// load the model associations from file
bool Epic::DAO::Nutrient::load()
{
    std::string value;
    std::string config_key = "nutrients";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Nutrient::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

// load the model from file
bool Epic::DAO::Nutrient::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Nutrient nutrient;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("CODE");
                expected.push_back("DESCRIPTION");
                expected.push_back("UNITS");
                if( Epic::Import::MatchedValues()("nutrients",expected,v))
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
            if(cnf.find("CODE",s))
            {
                long code = Conversion::IntegerString(s);
                nutrient.set_code(code);
            }
            if(cnf.find("DESCRIPTION",s))
            {
                nutrient.set_description(s);
            }
            if(cnf.find("UNITS",s))
            {
                nutrient.set_units(s);
            }

            if(!nutrient.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "nutrients" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}
