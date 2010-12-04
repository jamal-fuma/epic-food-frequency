#include "dao/CerealDAO.hpp"
#include "dao/Cereal.hpp"
#include "dao/Food.hpp"
#include "dao/Weight.hpp"
#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

// find a cereal given an id
bool Epic::CerealDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Cereal & cereal)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        cereal.set_id(id);
        cereal.set_food_id(m_find_by_id.column_int64(0));
        cereal.set_weight_id(m_find_by_id.column_int64(1));
        rc = cereal.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a cereal given a food_id
bool Epic::CerealDAO::DataAccess::find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Cereal & cereal)
{
    bool rc = false;
    m_find_by_food_id.bind_int64(1,food_id);
    if(SQLITE_ROW == m_find_by_food_id.step())
    {
        cereal.set_id(m_find_by_food_id.column_int64(0));
        cereal.set_food_id(food_id);
        cereal.set_weight_id(m_find_by_food_id.column_int64(1));
        rc = cereal.validate();
    }
    m_find_by_food_id.reset();
    return rc;
}

// save a cereal
bool Epic::CerealDAO::DataAccess::save(Epic::DAO::Cereal & cereal)
{
    m_insert.bind_int64(1,cereal.get_food_id());
    m_insert.bind_int64(2,cereal.get_weight_id());

    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        cereal.set_id(Epic::Database::last_insert_id());
        return cereal.validate();
    }
    return false;
}

// singleton versions

// find a cereal given an id
bool Epic::CerealDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::CerealDAO::DataAccess >::instance().find_by_id(id,cereal);
}

// find a cereal given a name
bool Epic::CerealDAO::find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::CerealDAO::DataAccess >::instance().find_by_food_id(food_id,cereal);
}

// save a cereal
bool Epic::CerealDAO::save(Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::CerealDAO::DataAccess >::instance().save(cereal);
}

// wire up saving the model to the DAO
bool Epic::DAO::Cereal::save()
{   
    return Epic::CerealDAO::save(*this);
}

// wire up finding the model using the DAO and a name
Epic::DAO::Cereal Epic::DAO::Cereal::find_by_food_id(sqlite3_int64 food_id)
{
    Epic::DAO::Cereal cereal;
    Epic::CerealDAO::find_by_food_id(food_id,cereal);
    if(!cereal.valid())
    {
        std::cerr << "invalid cereal " << cereal;
    }
    return cereal;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Cereal Epic::DAO::Cereal::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Cereal cereal;
    Epic::CerealDAO::find_by_id(id,cereal);
    return cereal;
}

// load the model from file
bool Epic::DAO::Cereal::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Cereal cereal;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("FOOD_CODE");
                expected.push_back("WEIGHT_ID");
                 if(Epic::Import::DBModel::same_header("cereals",expected,v))
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
                cereal.set_food_id(Epic::DAO::Food::find_by_name(s).get_id());
            }
            if(cnf.find("WEIGHT_ID",s))
            {
                cereal.set_weight_id(Epic::DAO::Weight::find_by_id(Epic::Conversion::IntegerString(s)).get_id());
            }

            if(!cereal.save())
            {
                std::ostringstream ss;
                ss << "Error in cereals import file: aborting on line :" << line << std::endl;
                Epic::Logging::error(ss.str());
                return false;
            }
        }
    }
    tr.commit();
    return true;
}

