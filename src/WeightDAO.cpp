#include "dao/WeightDAO.hpp"
#include "dao/Weight.hpp"

// find a weight given an id
bool Epic::WeightDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Weight & weight)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        weight.set_id(id);
        weight.set_amount(m_find_by_id.column_double(0));
        rc = weight.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// save a weight
bool Epic::WeightDAO::DataAccess::save(Epic::DAO::Weight & weight)
{
    m_insert.bind_double(1,weight.get_amount());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        weight.set_id(Epic::Database::last_insert_id());
        return weight.validate();
    }
    return false;
}

// singleton versions

// find a weight given an id
bool Epic::WeightDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Weight & weight)
{
    return Epic::Pattern::Singleton< Epic::WeightDAO::DataAccess >::instance().find_by_id(id,weight);
}

// save a weight
bool Epic::WeightDAO::save(Epic::DAO::Weight & weight)
{
    return Epic::Pattern::Singleton< Epic::WeightDAO::DataAccess >::instance().save(weight);
}

// wire up saving the model to the DAO
bool Epic::DAO::Weight::save()
{
    return Epic::WeightDAO::save(*this);
}

// wire up finding the model using the DAO and an id
Epic::DAO::Weight Epic::DAO::Weight::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Weight weight;
    Epic::WeightDAO::find_by_id(id,weight);
    return weight;
}

