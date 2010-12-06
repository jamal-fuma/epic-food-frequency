#include "dao/MealDAO.hpp"
#include "dao/MealFoodDAO.hpp"
#include "dao/Meal.hpp"
#include "dao/MealFood.hpp"
#include "import/Import.hpp"

// find a meal given an id
bool Epic::MealDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Meal & meal)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        meal.set_id(id);
        meal.set_name(m_find_by_id.column_text(0));
        meal.set_description(m_find_by_id.column_text(1));
        rc = meal.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a meal given a name
bool Epic::MealDAO::DataAccess::find_by_name(const std::string & name, Epic::DAO::Meal & meal)
{
    bool rc = false;
    m_find_by_name.bind_text(1,name);
    if(SQLITE_ROW == m_find_by_name.step())
    {
        meal.set_id(m_find_by_name.column_int64(0));
        meal.set_name(name);
        meal.set_description(m_find_by_name.column_text(1));
        rc = meal.validate();
    }
    m_find_by_name.reset();
    return rc;
}

// find all meals
bool Epic::MealDAO::DataAccess::find_all(std::vector<Epic::DAO::Meal> & meals)
{
    bool rc = false;
    Epic::DAO::Meal meal;
    rc = (SQLITE_ROW == m_find_all.step());
    if(!rc)
        return false;

    while(rc)
    {
        meal.set_id(m_find_all.column_int64(0));
        meal.set_name(m_find_all.column_text(1));
        meal.set_description(m_find_all.column_text(2));
        meal.validate();
        meals.push_back(meal);
        rc = (SQLITE_ROW == m_find_all.step());
    }
    m_find_all.reset();
    return true;
}


 // find all meals of the given type
bool Epic::MealDAO::DataAccess::find_all_by_type(Epic::DAO::MealFood::Types type, std::vector<Epic::DAO::Meal> & meals)
{
    bool rc = false;
    m_find_all_by_type.bind_int(1,type);
    Epic::DAO::Meal meal;
    rc = (SQLITE_ROW == m_find_all_by_type.step());
    if(!rc)
        return false;

    while(rc)
    {
        meal.set_id(m_find_all_by_type.column_int64(0));
        meal.set_name(m_find_all_by_type.column_text(1));
        meal.set_description(m_find_all_by_type.column_text(2));
        meals.push_back(meal);
        rc = (SQLITE_ROW == m_find_all_by_type.step());
    }
    m_find_all_by_type.reset();
    return true;
}

// find all meals of the given type
bool Epic::MealDAO::DataAccess::find_all_without_modifier(std::vector<Epic::DAO::Meal> & meals)
{
    bool rc = false;
    m_find_all_without_modifier.bind_int(1,Epic::DAO::MealFood::FryingFat);
    m_find_all_without_modifier.bind_int(2,Epic::DAO::MealFood::BakingFat);
    m_find_all_without_modifier.bind_int(3,Epic::DAO::MealFood::VisibleFat);
    m_find_all_without_modifier.bind_int(4,Epic::DAO::MealFood::Cereal);

    rc = (SQLITE_ROW == m_find_all_without_modifier.step());
    if(!rc)
        return false;

    for(Epic::DAO::Meal meal; (rc); rc = (SQLITE_ROW == m_find_all_without_modifier.step()))
    {
        meal.set_id(m_find_all_without_modifier.column_int64(0));
        meal.set_name(m_find_all_without_modifier.column_text(1));
        meal.set_description(m_find_all_without_modifier.column_text(2));
        meals.push_back(meal);
    }
    m_find_all_without_modifier.reset();
    return true;
}

// save a meal
bool Epic::MealDAO::DataAccess::save(Epic::DAO::Meal & meal)
{
    if(meal.description_empty() || meal.name_empty())
    {
        Epic::Logging::Error().log() << "Cant save a meal without a description and name" ;
        return false;
    }
    
    std::string name = meal.get_name();
    // check if it exists already
    if(!find_by_name(name,meal))
    {
	m_insert.bind_text(1,meal.get_name());
	m_insert.bind_text(2,meal.get_description());

        int rc = m_insert.step();
        m_insert.reset();
        if(rc == SQLITE_DONE)
        {
            meal.set_id(Epic::Database::last_insert_id());
            return meal.validate();
        }
    }

    // no-op as allready present in table
    return true;
}

// singleton versions

// find a meal given an id
bool Epic::MealDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Meal & meal)
{
    return Epic::Pattern::Singleton< Epic::MealDAO::DataAccess >::instance().find_by_id(id,meal);
}

// find a meal given a name
bool Epic::MealDAO::find_by_name(const std::string & name, Epic::DAO::Meal & meal)
{
    return Epic::Pattern::Singleton< Epic::MealDAO::DataAccess >::instance().find_by_name(name,meal);
}

// save a meal
bool Epic::MealDAO::save(Epic::DAO::Meal & meal)
{
    return Epic::Pattern::Singleton< Epic::MealDAO::DataAccess >::instance().save(meal);
}

// find all meals
bool Epic::MealDAO::find_all(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::Pattern::Singleton< Epic::MealDAO::DataAccess >::instance().find_all(meals);
}


 // find all meals of the given type
bool Epic::MealDAO::find_all_by_type(Epic::DAO::MealFood::Types type, std::vector<Epic::DAO::Meal> & meals)
{
    if(Epic::DAO::MealFood::Simple)
        return find_all_without_modifier(meals);

    return Epic::Pattern::Singleton< Epic::MealDAO::DataAccess >::instance().find_all_by_type(type,meals);
}

// find all meals of the given type
bool Epic::MealDAO::find_all_without_modifier(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::Pattern::Singleton< Epic::MealDAO::DataAccess >::instance().find_all_without_modifier(meals);
}

// wire up saving the model to the DAO
bool Epic::DAO::Meal::save()
{   
    return Epic::MealDAO::save(*this);
}

// wire up finding the model using the DAO and a name
Epic::DAO::Meal Epic::DAO::Meal::find_by_name(const std::string & name)
{
    Epic::DAO::Meal meal;
    Epic::MealDAO::find_by_name(name,meal);
    return meal;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Meal Epic::DAO::Meal::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Meal meal;
    Epic::MealDAO::find_by_id(id,meal);
    return meal;
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all_with_visible_fat(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::MealDAO::find_all_by_type(Epic::DAO::MealFood::VisibleFat,meals);
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all_with_baking_fat(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::MealDAO::find_all_by_type(Epic::DAO::MealFood::BakingFat,meals);
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all_with_cereal(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::MealDAO::find_all_by_type(Epic::DAO::MealFood::Cereal,meals);
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all_with_frying_fat(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::MealDAO::find_all_by_type(Epic::DAO::MealFood::FryingFat,meals);
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all_without_modifier(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::MealDAO::find_all_without_modifier(meals);
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all_foods(std::vector<Epic::DAO::MealFood> & meal_foods) const
{
    return Epic::MealFoodDAO::find_all_by_meal_id(this->get_id(),meal_foods);
}

// find all meals of the given type
bool Epic::DAO::Meal::find_all(std::vector<Epic::DAO::Meal> & meals)
{
    return Epic::MealDAO::find_all(meals);
}

// load the model associations from file
bool Epic::DAO::Meal::load()
{
    std::string value;
    std::string config_key = "meals";
    if(!Epic::Config::find(config_key,value))
    {
        Epic::Logging::Error().log() << "Config file lacks value for '" << config_key << "'" ;
        return false;
    }

    if(!Epic::DAO::Meal::load(value))
    {
        Epic::Logging::Error().log() <<  "Loading imports for '" << config_key << "' failed" ;
        return false;
    }

    Epic::Logging::Note().log() << "Loading imports for '" << config_key << "' completed" ;
    return true;
}


// load the model from file
bool Epic::DAO::Meal::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Meal meal;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("LINE NUMBER");
                expected.push_back("FFQNAME");
                expected.push_back("SGDESC");
                 if(Epic::Import::DBModel::same_header("meals",expected,v))
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
            if(cnf.find("FFQNAME",s))
            {
                meal.set_name(s);
            }
            if(cnf.find("SGDESC",s))
            {
                meal.set_description(s);
            }

            if(!meal.save())
            {
                Epic::Logging::Error().log() << "Error in [" << "meals" <<"] import file: [" << filename << "] aborting on line: " << line ;
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

