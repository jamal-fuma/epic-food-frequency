#include "dao/MealFoodDAO.hpp"
#include "dao/MealFood.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"

#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

// find all meal_foods with specified meal_id
bool Epic::MealFoodDAO::DataAccess::find_all_by_meal_id(sqlite3_int64 meal_id, std::vector<Epic::DAO::MealFood> & meal_foods)
{
    m_find_all_by_meal_id.bind_int64(1,meal_id);
    bool rc = (SQLITE_ROW == m_find_all_by_meal_id.step());

    if(rc)
    {
        Epic::DAO::MealFood meal_food;
        while( rc )
        {
            meal_food.set_meal_id(meal_id);
            meal_food.set_id(m_find_all_by_meal_id.column_int64(0));
            meal_food.set_food_id(m_find_all_by_meal_id.column_int64(1));
            meal_food.set_modifier(m_find_all_by_meal_id.column_int(2));   
            meal_food.set_amount(m_find_all_by_meal_id.column_double(3));    

            meal_foods.push_back(meal_food);

            rc = (SQLITE_ROW == m_find_all_by_meal_id.step());
        }
        rc = true;
    }
    m_find_all_by_meal_id.reset();
    return rc;
}

// find all meal_foods with specified modifier
bool Epic::MealFoodDAO::DataAccess::find_all_by_modifier(int modifier, std::vector<Epic::DAO::MealFood> & meal_foods)
{
    m_find_all_by_modifier.bind_int(1,modifier);
    bool rc = (SQLITE_ROW == m_find_all_by_modifier.step());

    if(rc)
    {
        Epic::DAO::MealFood meal_food;
        while( rc )
        {
            meal_food.set_modifier(modifier);
            meal_food.set_id(m_find_all_by_modifier.column_int64(0));
            meal_food.set_meal_id(m_find_all_by_modifier.column_int64(1));
            meal_food.set_food_id(m_find_all_by_modifier.column_int64(2));
            meal_food.set_amount(m_find_all_by_modifier.column_double(3));

            meal_foods.push_back(meal_food);

            rc = (SQLITE_ROW == m_find_all_by_modifier.step());
        }
        rc = true;
    }
    m_find_all_by_modifier.reset();
    return rc;
}

// find all meal_foods with specified modifier and meal
bool Epic::MealFoodDAO::DataAccess::find_all_by_meal_id_and_modifier(sqlite3_int64 meal_id, int modifier, std::vector<Epic::DAO::MealFood> & meal_foods)
{
    m_find_all_by_meal_id_and_modifier.bind_int64(1,meal_id);
    m_find_all_by_meal_id_and_modifier.bind_int(2,modifier);
    bool rc = (SQLITE_ROW == m_find_all_by_modifier.step());

    if(rc)
    {
        Epic::DAO::MealFood meal_food;
        while( rc )
        {
            meal_food.set_modifier(modifier);
            meal_food.set_meal_id(meal_id);
            meal_food.set_id(m_find_all_by_meal_id_and_modifier.column_int64(0));
            meal_food.set_food_id(m_find_all_by_meal_id_and_modifier.column_int64(1));
            meal_food.set_amount(m_find_all_by_meal_id_and_modifier.column_double(2));

            meal_foods.push_back(meal_food);

            rc = (SQLITE_ROW == m_find_all_by_meal_id_and_modifier.step());
        }
        rc = true;
    }
    m_find_all_by_modifier.reset();
    return rc;
}

// find meal_food with specified id
bool Epic::MealFoodDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::MealFood & meal_food)
{
    m_find_by_id.bind_int64(1,id);
    bool rc = (SQLITE_ROW == m_find_by_id.step());

    if(rc)
    {
        meal_food.set_id(id);
        meal_food.set_meal_id(m_find_by_id.column_int64(0));
        meal_food.set_food_id(m_find_by_id.column_int64(1));
        meal_food.set_modifier(m_find_by_id.column_int(2));
        meal_food.set_amount(m_find_by_id.column_double(3));
    }
    m_find_by_id.reset();
    return rc;
}

// save meal_food 
bool Epic::MealFoodDAO::DataAccess::save(Epic::DAO::MealFood & meal_food)
{

    m_insert.bind_int64(1,meal_food.get_meal_id());
    sqlite3_int64 id = meal_food.get_food_id();
    if(id)
    {
        m_insert.bind_int64(2,id);
    }
    else
    {
        m_insert.bind_null(2);
    }
    m_insert.bind_int(3,meal_food.get_modifier());
    m_insert.bind_double(4,meal_food.get_amount());

    bool rc = (SQLITE_DONE == m_insert.step());
    m_insert.reset();
    if(rc)
    {
        meal_food.set_id(Epic::Database::last_insert_id());
        return meal_food.validate();
    }
    return rc;
}

// singleton versions

// find a meal_food given an id
bool Epic::MealFoodDAO::find_by_id(sqlite3_int64 id, Epic::DAO::MealFood & meal_food)
{
    return Epic::Pattern::Singleton< Epic::MealFoodDAO::DataAccess >::instance().find_by_id(id,meal_food);
}


bool Epic::MealFoodDAO::find_all_by_meal_id(sqlite3_int64 meal_id, std::vector<Epic::DAO::MealFood> & meal_foods)
{
    return Epic::Pattern::Singleton< Epic::MealFoodDAO::DataAccess >::instance().find_all_by_meal_id(meal_id,meal_foods);
}

bool Epic::MealFoodDAO::find_all_by_modifier(int modifier, std::vector<Epic::DAO::MealFood> & meal_foods)
{
    return Epic::Pattern::Singleton< Epic::MealFoodDAO::DataAccess >::instance().find_all_by_modifier(modifier,meal_foods);
}

bool Epic::MealFoodDAO::find_all_by_meal_id_and_modifier(sqlite3_int64 meal_id, int modifier, std::vector<Epic::DAO::MealFood> & meal_foods)
{
    return Epic::Pattern::Singleton< Epic::MealFoodDAO::DataAccess >::instance().find_all_by_meal_id_and_modifier(meal_id,modifier,meal_foods);
}
// save a meal_food
bool Epic::MealFoodDAO::save(Epic::DAO::MealFood & meal_food)
{
    return Epic::Pattern::Singleton< Epic::MealFoodDAO::DataAccess >::instance().save(meal_food);
}

// model wiring
Epic::DAO::MealFood Epic::DAO::MealFood::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::MealFood meal_food;
    Epic::MealFoodDAO::find_by_id(id,meal_food);
    return meal_food;
}

bool Epic::DAO::MealFood::save()
{
    return Epic::MealFoodDAO::save(*this);
}

// load the model associations from file
bool Epic::DAO::MealFood::load()
{
    std::string value;
    std::string config_key = "meal_foods";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::MealFood::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}

// load the model from file
bool Epic::DAO::MealFood::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::MealFood meal_food;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("MEAL");
                expected.push_back("FOOD_CODE");
                expected.push_back("AMOUNT");
                expected.push_back("MODIFIER");
                if(Epic::Import::DBModel::same_header("meal_foods",expected,v))
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
            if(cnf.find("MEAL",s))
            {
                meal_food.set_meal_id(Epic::DAO::Meal::find_by_name(s).get_id());
            }
            if(cnf.find("FOOD_CODE",s))
            {
                meal_food.set_food_id(Epic::DAO::Food::find_by_name(s).get_id());
            }
            if(cnf.find("MODIFIER",s))
            {
                meal_food.set_modifier(s);
            }
            if(cnf.find("AMOUNT",s))
            {
                double d = Epic::Conversion::DecimalString(s);
                meal_food.set_amount(d);
            }

            if(!meal_food.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "meal_foods" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

