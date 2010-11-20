#include "dao/FoodDAO.hpp"
#include "dao/Food.hpp"

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

// save a food
bool Epic::FoodDAO::DataAccess::save(Epic::DAO::Food & food)
{
    if(food.name_empty())
    {
        Epic::Logging::error("Cant save a food a name\n");
        throw std::runtime_error("Cant save a food without a name\n");
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

// singleton versions

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
    Epic::FoodDAO::find_by_name(name,food);
    return food;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Food Epic::DAO::Food::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Food food;
    Epic::FoodDAO::find_by_id(id,food);
    return food;
}
