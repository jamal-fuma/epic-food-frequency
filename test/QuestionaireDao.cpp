#include "QuestionaireDao.hpp"
#include "Questionaire.hpp"

// find a questionaire given an id
bool Test::Questionaire::DataAccess::find(sqlite3_int64 id, Test::Questionaire::Questionaire & questionaire)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        questionaire.set_id(id);
        questionaire.set_filename(m_find_by_id.column_text(0));
        rc = questionaire.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a questionaire given a filename
bool Test::Questionaire::DataAccess::find(const std::string & filename, Test::Questionaire::Questionaire & questionaire)
{
    bool rc = false;
    m_find_by_filename.bind_text(1,filename);
    if(SQLITE_ROW == m_find_by_filename.step())
    {
        questionaire.set_id(m_find_by_filename.column_int64(0));
        questionaire.set_filename(filename);
        rc = questionaire.validate();
    }
    m_find_by_filename.reset();
    return rc;
}

// save a questionaire
bool Test::Questionaire::DataAccess::save(Test::Questionaire::Questionaire & questionaire)
{
    if(questionaire.filename_empty())
    {
        Epic::Logging::error("Cant save a questionaire without a filename\n");
        throw std::runtime_error("Cant save a questionaire without a filename\n");
    }

    std::string fname = questionaire.get_filename();

    // check if it exists already
    if(!find(fname,questionaire))
    {
        m_insert.bind_text(1,fname);
        int rc = m_insert.step();
        m_insert.reset();
        if(rc == SQLITE_DONE)
        {
            questionaire.set_id(Epic::Database::last_insert_id());
            return questionaire.validate();
        }
    }

    // no-op as allready present in table
    return true;
}

// singleton versions

// find a questionaire given an id
bool Test::Questionaire::find_by_id(sqlite3_int64 id, Test::Questionaire::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Test::Questionaire::DataAccess >::instance().find(id,questionaire);
}

// find a questionaire given a filename
bool Test::Questionaire::find_by_filename(const std::string & filename, Test::Questionaire::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Test::Questionaire::DataAccess >::instance().find(filename,questionaire);
}

// save a questionaire
bool Test::Questionaire::save(Test::Questionaire::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Test::Questionaire::DataAccess >::instance().save(questionaire);
}


// wire up saving the model to the DAO
bool Test::Questionaire::Questionaire::save()
{   
    return Test::Questionaire::save(*this);
}

// wire up finding the model using the DAO and a filename
Test::Questionaire::Questionaire Test::Questionaire::Questionaire::find_by_filename(const std::string & filename)
{
    Test::Questionaire::Questionaire q;
    Test::Questionaire::find_by_filename(filename,q);
    return q;
}

// wire up finding the model using the DAO and an id
Test::Questionaire::Questionaire Test::Questionaire::Questionaire::find_by_id(sqlite3_int64 id)
{
    Test::Questionaire::Questionaire q;
    Test::Questionaire::find_by_id(id,q);
    return q;
}
