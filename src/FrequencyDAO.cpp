#include "dao/FrequencyDAO.hpp"
#include "dao/Frequency.hpp"

// find a frequency given an id
bool Epic::FrequencyDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Frequency & frequency)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        frequency.set_id(id);
        frequency.set_amount(m_find_by_id.column_double(0));
        rc = frequency.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// save a frequency
bool Epic::FrequencyDAO::DataAccess::save(Epic::DAO::Frequency & frequency)
{
    m_insert.bind_double(1,frequency.get_amount());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        frequency.set_id(Epic::Database::last_insert_id());
        return frequency.validate();
    }
    return false;
}

// singleton versions

// find a frequency given an id
bool Epic::FrequencyDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Frequency & frequency)
{
    return Epic::Pattern::Singleton< Epic::FrequencyDAO::DataAccess >::instance().find_by_id(id,frequency);
}

// save a frequency
bool Epic::FrequencyDAO::save(Epic::DAO::Frequency & frequency)
{
    return Epic::Pattern::Singleton< Epic::FrequencyDAO::DataAccess >::instance().save(frequency);
}

// wire up saving the model to the DAO
bool Epic::DAO::Frequency::save()
{
    return Epic::FrequencyDAO::save(*this);
}

// wire up finding the model using the DAO and an id
Epic::DAO::Frequency Epic::DAO::Frequency::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Frequency frequency;
    Epic::FrequencyDAO::find_by_id(id,frequency);
    return frequency;
}
