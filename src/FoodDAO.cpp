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

#include "FoodDAO.hpp"
#include "dao/Food.hpp"
#include "dao/Nutrient.hpp"

#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

// find a food given an id
bool Epic::FoodDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Food & food)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        food.set_id(id);
        food.set_name(m_find_by_id.column_text(0));
        food.set_description(m_find_by_id.column_text(1));
        rc = food.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a food given a name
bool Epic::FoodDAO::DataAccess::find_by_name(const std::string & name, Epic::DAO::Food & food)
{
    bool rc = false;
    m_find_by_name.bind_text(1,name);
    if(SQLITE_ROW == m_find_by_name.step())
    {
        food.set_id(m_find_by_name.column_int64(0));
        food.set_name(name);
        food.set_description(m_find_by_name.column_text(1));
        rc = food.validate();
    }
    m_find_by_name.reset();
    return rc;
}

// find all foods
bool Epic::FoodDAO::DataAccess::find_all(std::vector<Epic::DAO::Food> & foods)
{
    bool rc = false;
    Epic::DAO::Food food;
    rc = (SQLITE_ROW == m_find_all.step());
    if(!rc)
        return false;

    while(rc)
    {
        food.set_id(m_find_all.column_int64(0));
        food.set_name(m_find_all.column_text(1));
        food.set_description(m_find_all.column_text(2));
        food.validate();
        foods.push_back(food);
        rc = (SQLITE_ROW == m_find_all.step());
    }
    m_find_all.reset();
    return true;
}



// save a food
bool Epic::FoodDAO::DataAccess::save(Epic::DAO::Food & food)
{
    if(food.name_empty())
    {
        Epic::Logging::Error().log() <<  "Attempted to save food without a food_code name" ;
        return false;
    }
    
    std::string name = food.get_name();
    // check if it exists already
    if(!find_by_name(name,food))
    {
	m_insert.bind_text(1,food.get_name());
	m_insert.bind_text(2,food.get_description());

        int rc = m_insert.step();
        m_insert.reset();
        if(rc == SQLITE_DONE)
        {
            food.set_id(Epic::Database::last_insert_id());
            return food.validate();
        }
    }

    // no-op as allready present in table
    return true;
}

// associate a food with a nutrient
bool Epic::FoodDAO::DataAccess::attach(const Epic::DAO::Food & food, const Epic::DAO::Nutrient & nutrient, double amount)
{
    if(!food.valid())
    {
        Epic::Logging::Error().log() <<  "Attempted to associate nutrient with invalid food" ;
        return false;
    }
    if(!nutrient.valid())
    {
        Epic::Logging::Error().log() <<  "Attempted to associate food with invalid nutrient" ;
        return false;
    }

    m_attach.bind_int64(1,food.get_id());
    m_attach.bind_int64(2,nutrient.get_id());
    m_attach.bind_double(3,amount);
    
    int rc = m_attach.step();
    m_attach.reset();
    return(rc == SQLITE_DONE);
}

// find all nutrients associated with a food
bool Epic::FoodDAO::DataAccess::find_nutrients(const Epic::DAO::Food & food, std::vector<Epic::DAO::FoodNutrient> & nutrients)
{
    if(!food.valid())
        return false;

    m_find_nutrients_by_food_id.bind_int64(1,food.get_id());
    bool rc = (SQLITE_ROW == m_find_nutrients_by_food_id.step());
    std::vector<Epic::DAO::FoodNutrient> tmp;
    if(rc)
    {
        tmp.swap(nutrients);
        Epic::DAO::FoodNutrient nutrient;
        for(sqlite3_int64 id = food.get_id(); rc; )
        {
            nutrient.set_food_id(id);
            nutrient.set_nutrient_id(m_find_nutrients_by_food_id.column_int64(0));    
            nutrient.set_amount(m_find_nutrients_by_food_id.column_double(1));    
            nutrient.set_nutrient_code(m_find_nutrients_by_food_id.column_int(2));    

            nutrients.push_back(nutrient);

            rc = (SQLITE_ROW == m_find_nutrients_by_food_id.step());
        }
        rc = true;
    }
    m_find_nutrients_by_food_id.reset();
    return rc;
}
// singleton versions

// associate a food with a nutrient
bool Epic::FoodDAO::attach(const Epic::DAO::Food & food, const Epic::DAO::Nutrient & nutrient, double amount)
{
    return Epic::Pattern::Singleton< Epic::FoodDAO::DataAccess >::instance().attach(food,nutrient,amount);
}

// find all nutrients associated with a food
bool Epic::FoodDAO::find_nutrients(const Epic::DAO::Food & food, std::vector<Epic::DAO::FoodNutrient> & nutrients)
{
    return Epic::Pattern::Singleton< Epic::FoodDAO::DataAccess >::instance().find_nutrients(food,nutrients);
} 

// find a food given an id
bool Epic::FoodDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::FoodDAO::DataAccess >::instance().find_by_id(id,food);
}

// find a food given a name
bool Epic::FoodDAO::find_by_name(const std::string & name, Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::FoodDAO::DataAccess >::instance().find_by_name(name,food);
}

// find all foods
bool Epic::FoodDAO::find_all(std::vector<Epic::DAO::Food> & foods)
{
    return Epic::Pattern::Singleton< Epic::FoodDAO::DataAccess >::instance().find_all(foods);
}

// save a food
bool Epic::FoodDAO::save(Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::FoodDAO::DataAccess >::instance().save(food);
}

// wire up saving the model to the DAO
bool Epic::DAO::Food::save()
{   
    return Epic::FoodDAO::save(*this);
}

// wire up finding the model using the DAO and a name
Epic::DAO::Food Epic::DAO::Food::find_by_name(const std::string & name)
{
    Epic::DAO::Food food;
    Epic::FoodDAO::find_by_name(Epic::Util::c_trim(name),food);
    if(!food.valid())
    {
        Epic::Logging::Error().log() <<  "Attempted to locate food with code [" << Epic::Util::c_trim(name) << "] failed";
    }
    return food;
}

// find all foods
bool Epic::DAO::Food::find_all(std::vector<Epic::DAO::Food> & foods)
{
    return Epic::FoodDAO::find_all(foods);
}

// wire up finding the model using the DAO and an id
Epic::DAO::Food Epic::DAO::Food::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Food food;
    Epic::FoodDAO::find_by_id(id,food);
    return food;
}

// wire up attaching nutrients to foods
bool Epic::DAO::Food::attach( Epic::DAO::Nutrient & nutrient, double amount)
{
    return  Epic::FoodDAO::attach(*this,nutrient,amount);
}

// wire up retrieving nutrients for this food
bool Epic::DAO::Food::find_nutrients(std::vector<FoodNutrient> & nutrients) const
{
    return  Epic::FoodDAO::find_nutrients(*this,nutrients);
}

// load the model associations from file
bool Epic::DAO::Food::load()
{
    std::string value;
    std::string config_key = "foods";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Food::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

// load the model from file
bool Epic::DAO::Food::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Food food;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("FOOD_CODE");
                expected.push_back("DESC");
                if(Epic::Import::DBModel::same_header("foods",expected,v))
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
                food.set_name(s);
            }
            if(cnf.find("DESC",s))
            {
                food.set_description(s);
            }

            if(!food.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "foods" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

// load the model associations from file
bool Epic::DAO::FoodNutrient::load()
{
    std::string value;
    std::string config_key = "food_nutrients";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::FoodNutrient::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

bool Epic::DAO::FoodNutrient::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Food food;
    Epic::DAO::Nutrient nutrient;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("FOOD_CODE");
                expected.push_back("NUTRIENT");
                expected.push_back("QUANTITY");
                if(Epic::Import::DBModel::same_header("food_nutrients",expected,v))
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
            double quantity = 0.0;
            if(cnf.find("FOOD_CODE",s) && s != food.get_name())
            {
                food = Epic::DAO::Food::find_by_name(s);
            }
        
            bool log = false;
            if(cnf.find("NUTRIENT",s) )
            {
                // nutrient code
                long code = Epic::Conversion::IntegerString(s);
                nutrient = Epic::DAO::Nutrient::find_by_code(code);
            }

            if(cnf.find("QUANTITY",s) )
            {
                // quantity
                quantity = Conversion::NutrientQuantity(s);
            }
            
            if(!food.attach(nutrient,quantity))
            {
                Epic::Logging::Error().log() << "Error in [" << "food_nutrients" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

