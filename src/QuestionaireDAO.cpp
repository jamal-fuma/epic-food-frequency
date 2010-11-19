#include "dao/QuestionaireDAO.hpp"
#include "dao/Questionaire.hpp"

// find a questionaire given an id
bool Epic::QuestionaireDAO::DataAccess::find(sqlite3_int64 id, Epic::DAO::Questionaire & questionaire)
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
bool Epic::QuestionaireDAO::DataAccess::find(const std::string & filename, Epic::DAO::Questionaire & questionaire)
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
bool Epic::QuestionaireDAO::DataAccess::save(Epic::DAO::Questionaire & questionaire)
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
bool Epic::QuestionaireDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().find(id,questionaire);
}

// find a questionaire given a filename
bool Epic::QuestionaireDAO::find_by_filename(const std::string & filename, Epic::DAO::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().find(filename,questionaire);
}

// save a questionaire
bool Epic::QuestionaireDAO::save(Epic::DAO::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().save(questionaire);
}


// wire up saving the model to the DAO
bool Epic::DAO::Questionaire::save()
{   
    return Epic::QuestionaireDAO::save(*this);
}

// wire up finding the model using the DAO and a filename
Epic::DAO::Questionaire Epic::DAO::Questionaire::find_by_filename(const std::string & filename)
{
    Epic::DAO::Questionaire q;
    Epic::QuestionaireDAO::find_by_filename(filename,q);
    return q;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Questionaire Epic::DAO::Questionaire::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Questionaire q;
    Epic::QuestionaireDAO::find_by_id(id,q);
    return q;
}
