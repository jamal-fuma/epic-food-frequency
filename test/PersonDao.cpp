#include "PersonDao.hpp"
#include "Person.hpp"

// find a person given an id
bool Test::Person::DataAccess::find(sqlite3_int64 id, Test::Person::Person & person)
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
bool Test::Person::DataAccess::find(const std::string & reference, Test::Person::Person & person)
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
bool Test::Person::DataAccess::save(Test::Person::Person & person)
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

// singleton versions

// find a person given an id
bool Test::Person::find_by_id(sqlite3_int64 id, Test::Person::Person & person)
{
    return Epic::Pattern::Singleton< Test::Person::DataAccess >::instance().find(id,person);
}

// find a person given a reference
bool Test::Person::find_by_reference(const std::string & reference, Test::Person::Person & person)
{
    return Epic::Pattern::Singleton< Test::Person::DataAccess >::instance().find(reference,person);
}

// save a person
bool Test::Person::save(Test::Person::Person & person)
{
    return Epic::Pattern::Singleton< Test::Person::DataAccess >::instance().save(person);
}


// wire up saving the model to the DAO
bool Test::Person::Person::save()
{   
    return Test::Person::save(*this);
}

// wire up finding the model using the DAO and a reference
Test::Person::Person Test::Person::Person::find_by_reference(const std::string & reference)
{
    Test::Person::Person q;
    Test::Person::find_by_reference(reference,q);
    return q;
}

// wire up finding the model using the DAO and an id
Test::Person::Person Test::Person::Person::find_by_id(sqlite3_int64 id)
{
    Test::Person::Person q;
    Test::Person::find_by_id(id,q);
    return q;
}
