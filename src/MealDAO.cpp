#include "dao/MealDAO.hpp"
#include "dao/Meal.hpp"

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

// save a meal
bool Epic::MealDAO::DataAccess::save(Epic::DAO::Meal & meal)
{
    if(meal.description_empty() || meal.name_empty())
    {
        Epic::Logging::error("Cant save a meal without a description and name\n");
        throw std::runtime_error("Cant save a meal without a description and name\n");
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
