#include "dao/PersonDAO.hpp"
#include "dao/Person.hpp"
#include "dao/Weight.hpp"
#include "dao/MealFood.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/Portion.hpp"
#include "dao/Frequency.hpp"
#include "dao/Milk.hpp"
#include "dao/Cereal.hpp"

// find a person given an id
bool Epic::PersonDAO::DataAccess::find(sqlite3_int64 id, Epic::DAO::Person & person)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        person.set_id(id);
        person.set_reference(m_find_by_id.column_text(0));
        rc = person.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a person given a reference
bool Epic::PersonDAO::DataAccess::find(const std::string & reference, Epic::DAO::Person & person)
{
    bool rc = false;
    m_find_by_reference.bind_text(1,reference);
    if(SQLITE_ROW == m_find_by_reference.step())
    {
        person.set_id(m_find_by_reference.column_int64(0));
        person.set_reference(reference);
        rc = person.validate();
    }
    m_find_by_reference.reset();
    return rc;
}

// save a person
bool Epic::PersonDAO::DataAccess::save(Epic::DAO::Person & person)
{
    if(person.reference_empty())
    {
        Epic::Logging::error("Cant save a person without a reference\n");
        throw std::runtime_error("Cant save a person without a reference\n");
    }

    std::string fname = person.get_reference();

    // check if it exists already
    if(!find(fname,person))
    {
        m_insert.bind_text(1,fname);
        int rc = m_insert.step();
        m_insert.reset();
        if(rc == SQLITE_DONE)
        {
            person.set_id(Epic::Database::last_insert_id());
            return person.validate();
        }
    }

    // no-op as allready present in table
    return true;
}

// associate a person with a weight
bool Epic::PersonDAO::DataAccess::attach_weight(const Epic::DAO::Person & person,const Epic::DAO::Weight & weight)
{
    if(!weight.valid() || !person.valid())
    {
        std::ostringstream ss;
        ss << "Cant attach a weight to a person unless both are valid()\n";
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    bool rc = false;
    m_attach_weight.bind_int64(1,person.get_id());
    m_attach_weight.bind_int64(2,weight.get_id());
    m_attach_weight.bind_int(3,Epic::DAO::MealFood::VisibleFat);
    rc = (SQLITE_DONE == m_attach_weight.step());
    m_attach_weight.reset();
    return rc;
}

bool Epic::PersonDAO::DataAccess::attach_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food, const Epic::DAO::MealFood::Types type)
{
    if(!food.valid() || !person.valid())
    {
        std::ostringstream ss;
        ss << "Cant attach a fat type to a person unless both are valid()\n";
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    bool rc = false;
    m_attach_fat.bind_int64(1,person.get_id());
    m_attach_fat.bind_int64(2,food.get_id());
    m_attach_fat.bind_int(3,type);
    rc = (SQLITE_DONE == m_attach_fat.step());
    m_attach_fat.reset();
    return rc;
}

bool Epic::PersonDAO::DataAccess::attach_cereal(const Epic::DAO::Person & person,const Epic::DAO::Cereal & cereal)
{
    if(!cereal.valid() || !person.valid())
    {
        std::ostringstream ss;
        ss << "Cant attach a cereal to a person unless both are valid()\n";
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    bool rc = false;
    m_attach_cereal.bind_int64(1,person.get_id());
    m_attach_cereal.bind_int64(2,cereal.get_food_id());
    m_attach_cereal.bind_int64(3,cereal.get_weight_id());
    m_attach_cereal.bind_int(4,Epic::DAO::MealFood::Cereal);
    rc = (SQLITE_DONE == m_attach_cereal.step());
    m_attach_cereal.reset();
    return rc;
}

// associate a person with a milk food code and portion
bool Epic::PersonDAO::DataAccess::attach_milk(const Epic::DAO::Person & person,const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion)
{
    if(!milk.valid() || !portion.valid() || !person.valid())
    {
        std::ostringstream ss;
        ss << "Cant attach a milk portion to a person unless person and milk and portion are valid()\n";
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    bool rc = false;
    m_attach_food.bind_int64(1,person.get_id());
    m_attach_food.bind_int64(2,milk.get_food_id());
    m_attach_food.bind_int64(3,portion.get_id());
    rc = (SQLITE_DONE == m_attach_food.step());
    m_attach_food.reset();
    return rc;
}

// associate a person with a meal frequency
bool Epic::PersonDAO::DataAccess::attach_meal(const Epic::DAO::Person & person,const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency)
{
    if(!meal.valid() || !frequency.valid() || !person.valid())
    {
        std::ostringstream ss;
        ss << "Cant attach a meal frequency to a person unless person and meal and frequency are valid()\n";
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str());
    }

    bool rc = false;
    m_attach_frequency.bind_int64(1,person.get_id());
    m_attach_frequency.bind_int64(2,meal.get_id());
    m_attach_frequency.bind_int64(3,frequency.get_id());
    rc = (SQLITE_DONE == m_attach_frequency.step());
    m_attach_frequency.reset();
    return rc;
}


// associate a person with a baking fat
bool Epic::PersonDAO::DataAccess::attach_baking_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return attach_fat(person,food,Epic::DAO::MealFood::BakingFat);
}

// associate a person with a frying fat
bool Epic::PersonDAO::DataAccess::attach_frying_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return attach_fat(person,food,Epic::DAO::MealFood::FryingFat);
}


// associate a person with a meal frequency

// singleton versions

// find a person given an id
bool Epic::PersonDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().find(id,person);
}

// find a person given a reference
bool Epic::PersonDAO::find_by_reference(const std::string & reference, Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().find(reference,person);
}

// save a person
bool Epic::PersonDAO::save(Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().save(person);
}

// associate a person with a weight
bool Epic::PersonDAO::attach_weight(const Epic::DAO::Person & person,const Epic::DAO::Weight & weight)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_weight(person,weight);
}

// associate a person with a baking_fat
bool Epic::PersonDAO::attach_baking_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_baking_fat(person,food);
}

// associate a person with a frying_fat
bool Epic::PersonDAO::attach_frying_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_frying_fat(person,food);
}

// associate a person with a cereal
bool Epic::PersonDAO::attach_cereal(const Epic::DAO::Person & person,const Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_cereal(person,cereal);
}
// associate a person with a milk food code and portion
bool Epic::PersonDAO::attach_milk(const Epic::DAO::Person & person,const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_milk(person,milk,portion);
}

// associate a person with a meal frequency
bool Epic::PersonDAO::attach_meal(const Epic::DAO::Person & person,const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_meal(person,meal,frequency);
}

// wire up saving the model to the DAO
bool Epic::DAO::Person::save()
{   
    return Epic::PersonDAO::save(*this);
}

// wire up associate a person with a visible fat weighting using the model
bool Epic::DAO::Person::attach_visible_fat(const Epic::DAO::Weight & weight)
{
    return Epic::PersonDAO::attach_weight(*this,weight);
}

// wire up associate a person with a baking_fat
bool Epic::DAO::Person::attach_baking_fat(const Epic::DAO::Food & food)
{
    return Epic::PersonDAO::attach_baking_fat(*this,food);
}

// wire up associate a person with a frying_fat
bool Epic::DAO::Person::attach_frying_fat(const Epic::DAO::Food & food)
{
    return Epic::PersonDAO::attach_frying_fat(*this,food);
}

// wire up associate a person with a cereal
bool Epic::DAO::Person::attach_cereal(const Epic::DAO::Cereal & cereal)
{
    return Epic::PersonDAO::attach_cereal(*this,cereal);
}

// associate a person with a milk food code and portion
bool Epic::DAO::Person::attach_milk(const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion)
{
    return Epic::PersonDAO::attach_milk(*this,milk,portion);
}

// associate a person with a meal frequency
bool Epic::DAO::Person::attach_meal(const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency)
{
    return Epic::PersonDAO::attach_meal(*this,meal,frequency);
}

// wire up finding the model using the DAO and a reference
Epic::DAO::Person Epic::DAO::Person::find_by_reference(const std::string & reference)
{
    Epic::DAO::Person person;
    Epic::PersonDAO::find_by_reference(reference,person);
    return person;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Person Epic::DAO::Person::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Person person;
    Epic::PersonDAO::find_by_id(id,person);
    return person;
}
