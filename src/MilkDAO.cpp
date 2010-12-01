#include "dao/MilkDAO.hpp"
#include "dao/Milk.hpp"
#include "dao/Food.hpp"
#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

// find a milk given an id
bool Epic::MilkDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Milk & milk)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        milk.set_id(id);
        milk.set_food_id(m_find_by_id.column_int64(0));
        milk.set_code(m_find_by_id.column_int(1));
        rc = milk.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a milk given a food_id
bool Epic::MilkDAO::DataAccess::find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Milk & milk)
{
    bool rc = false;
    m_find_by_food_id.bind_int64(1,food_id);
    if(SQLITE_ROW == m_find_by_food_id.step())
    {
        milk.set_id(m_find_by_food_id.column_int64(0));
        milk.set_food_id(food_id);
        milk.set_code(m_find_by_food_id.column_int(1));
        rc = milk.validate();
    }
    m_find_by_food_id.reset();
    return rc;
}

// find a milk given a code
bool Epic::MilkDAO::DataAccess::find_by_code(int code, Epic::DAO::Milk & milk)
{
    bool rc = false;
    m_find_by_code.bind_int(1,code);
    if(SQLITE_ROW == m_find_by_code.step())
    {
        milk.set_id(m_find_by_code.column_int64(0));
        milk.set_food_id(m_find_by_code.column_int64(1));
        milk.set_code(code);
        rc = milk.validate();
    }
    m_find_by_code.reset();
    return rc;
}

// save a milk
bool Epic::MilkDAO::DataAccess::save(Epic::DAO::Milk & milk)
{
    m_insert.bind_int64(1,milk.get_food_id());
    m_insert.bind_int64(2,milk.get_code());

    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        milk.set_id(Epic::Database::last_insert_id());
        return milk.validate();
    }
    return false;
}

// singleton versions

// find a milk given an id
bool Epic::MilkDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Milk & milk)
{
    return Epic::Pattern::Singleton< Epic::MilkDAO::DataAccess >::instance().find_by_id(id,milk);
}

// find a milk given a food_id
bool Epic::MilkDAO::find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Milk & milk)
{
    return Epic::Pattern::Singleton< Epic::MilkDAO::DataAccess >::instance().find_by_food_id(food_id,milk);
}

// find a milk given a code
bool Epic::MilkDAO::find_by_code(int code, Epic::DAO::Milk & milk)
{
    return Epic::Pattern::Singleton< Epic::MilkDAO::DataAccess >::instance().find_by_code(code,milk);
}

// save a milk
bool Epic::MilkDAO::save(Epic::DAO::Milk & milk)
{
    return Epic::Pattern::Singleton< Epic::MilkDAO::DataAccess >::instance().save(milk);
}

// wire up saving the model to the DAO
bool Epic::DAO::Milk::save()
{   
    return Epic::MilkDAO::save(*this);
}

// wire up finding the model using the DAO and a food_id
Epic::DAO::Milk Epic::DAO::Milk::find_by_food_id(sqlite3_int64 food_id)
{
    Epic::DAO::Milk milk;
    Epic::MilkDAO::find_by_food_id(food_id,milk);
    return milk;
}

// wire up finding the model using the DAO and a code
Epic::DAO::Milk Epic::DAO::Milk::find_by_code(int code)
{
    Epic::DAO::Milk milk;
    Epic::MilkDAO::find_by_code(code,milk);
    return milk;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Milk Epic::DAO::Milk::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Milk milk;
    Epic::MilkDAO::find_by_id(id,milk);
    return milk;
}

// load the model from file
bool Epic::DAO::Milk::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Milk milk;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("CODE");
                expected.push_back("FOOD_CODE");
                if(Epic::Import::DBModel::same_header("milks",expected,v))
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
                milk.set_food_id(Epic::DAO::Food::find_by_name(s).get_id());
            }

            if(cnf.find("CODE",s))
            {
                milk.set_code(Epic::Conversion::IntegerString(s));
            }

            if(!milk.save())
            {
                std::ostringstream ss;
                ss << "Error in milks import file: aborting on line :" << line << std::endl;
                Epic::Logging::error(ss.str());
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

