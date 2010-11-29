#include "dao/PortionDAO.hpp"
#include "dao/Portion.hpp"
#include "conversion/Conversion.hpp"
#include "import/Import.hpp"

// find a portion given an id
bool Epic::PortionDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        portion.set_id(id);
        portion.set_amount(m_find_by_id.column_double(0));
        rc = portion.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// save a portion
bool Epic::PortionDAO::DataAccess::save(Epic::DAO::Portion & portion)
{
    m_insert.bind_double(1,portion.get_amount());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
    {
        portion.set_id(Epic::Database::last_insert_id());
        return portion.validate();
    }
    return false;
}

// singleton versions

// find a portion given an id
bool Epic::PortionDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion)
{
    return Epic::Pattern::Singleton< Epic::PortionDAO::DataAccess >::instance().find_by_id(id,portion);
}

// save a portion
bool Epic::PortionDAO::save(Epic::DAO::Portion & portion)
{
    return Epic::Pattern::Singleton< Epic::PortionDAO::DataAccess >::instance().save(portion);
}

// wire up saving the model to the DAO
bool Epic::DAO::Portion::save()
{
    return Epic::PortionDAO::save(*this);
}

// wire up finding the model using the DAO and an id
Epic::DAO::Portion Epic::DAO::Portion::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Portion portion;
    Epic::PortionDAO::find_by_id(id,portion);
    return portion;
}

// load the model from file
bool Epic::DAO::Portion::load(const std::string & filename)
{
    Epic::Import::CSVReader rdr;

    if(!rdr.open(filename))
    {
        return false;
    }

    Epic::Import::str_vector_t v,h;
    Epic::Config::Config cnf;
    Epic::DAO::Portion portion;

    Epic::Database::Transaction tr;
    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                Epic::Import::str_vector_t expected;
                expected.push_back("CODE");
                expected.push_back("VALUE");
                if(Epic::Import::DBModel::same_header("portions",expected,v))
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
            if(cnf.find("VALUE",s))
            {
                portion.set_amount( Epic::Conversion::DecimalString(s));
            }

            if(!portion.save())
            {
                std::ostringstream ss;
                ss << "Error in portions import file: aborting on line :" << line << std::endl;
                Epic::Logging::error(ss.str());
                return false;
            }
        }
    }
    tr.commit();
    return true;
}


