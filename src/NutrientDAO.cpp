#include "dao/NutrientDAO.hpp"
#include "dao/Nutrient.hpp"

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

// save a nutrient
bool Epic::NutrientDAO::DataAccess::save(Epic::DAO::Nutrient & nutrient)
{
    if(nutrient.description_empty() || nutrient.units_empty())
    {
        Epic::Logging::error("Cant save a nutrient without a description and units\n");
        throw std::runtime_error("Cant save a nutrient without a description and units\n");
    }
    
    int code = nutrient.get_code();
    if(code <= 0 )
    {
        Epic::Logging::error("Cant save a nutrient without a code greater then zero\n");
        throw std::runtime_error("Cant save a nutrient without a code greater then zero\n");
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
