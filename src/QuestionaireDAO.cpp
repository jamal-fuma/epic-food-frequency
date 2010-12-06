#include "dao/QuestionaireDAO.hpp"
#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"

// find all people associated with a questionaire
bool Epic::QuestionaireDAO::DataAccess::find_people(const Epic::DAO::Questionaire & questionaire, std::vector<Epic::DAO::Person> & people)
{
    sqlite3_int64 id = questionaire.get_id();

    // no point in a query doomed to failure
    if(0 == id)
        return false;

    bool rc = false;
    m_find_people_by_questionaire_id.bind_int64(1,id);

    rc = (SQLITE_ROW == m_find_people_by_questionaire_id.step());
    if(rc)
    {
        while(rc)
        {
            people.push_back(Epic::DAO::Person::find_by_id(m_find_people_by_questionaire_id.column_int64(0)));
            rc = (SQLITE_ROW == m_find_people_by_questionaire_id.step());
        }
        rc = true;
    }
    m_find_people_by_questionaire_id.reset();
    return rc;
}

// find a questionaire given an id
bool Epic::QuestionaireDAO::DataAccess::find_by_id(sqlite3_int64 id, Epic::DAO::Questionaire & questionaire)
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
bool Epic::QuestionaireDAO::DataAccess::find_by_filename(const std::string & filename, Epic::DAO::Questionaire & questionaire)
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
        Epic::Logging::Error().log() << "Cant save a questionaire without a filename";
        return false;
    }

    std::string fname = questionaire.get_filename();

    // check if it exists already
    if(!find_by_filename(fname,questionaire))
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

// associate a person with a questionaire
bool Epic::QuestionaireDAO::DataAccess::attach(const Epic::DAO::Questionaire & questionaire, const Epic::DAO::Person & person)
{
    if(!questionaire.valid())
    {
         Epic::Logging::Error().log() << "Cant attach a person to an invalid questionaire";
         return false;
    }
    if(!person.valid())
    {
         Epic::Logging::Error().log() << "Cant attach a invalid person to a questionaire";
         return false;
    }
    bool rc = false;
    m_attach.bind_int64(1,questionaire.get_id());
    m_attach.bind_int64(2,person.get_id());
    rc = (SQLITE_DONE == m_attach.step());
    m_attach.reset();
    return rc;
}


// singleton versions

// find a questionaire given an id
bool Epic::QuestionaireDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().find_by_id(id,questionaire);
}

// find a questionaire given a filename
bool Epic::QuestionaireDAO::find_by_filename(const std::string & filename, Epic::DAO::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().find_by_filename(filename,questionaire);
}

// save a questionaire
bool Epic::QuestionaireDAO::save(Epic::DAO::Questionaire & questionaire)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().save(questionaire);
}

// attach a person to a questionaire
bool Epic::QuestionaireDAO::attach(const Epic::DAO::Questionaire & questionaire, const Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().attach(questionaire,person);
}

// find all people associated with a questionaire
bool Epic::QuestionaireDAO::find_people(const Epic::DAO::Questionaire & questionaire, std::vector<Epic::DAO::Person> & people)
{
    return Epic::Pattern::Singleton< Epic::QuestionaireDAO::DataAccess >::instance().find_people(questionaire,people);
}

// wire up saving the model to the DAO
bool Epic::DAO::Questionaire::save()
{
    return Epic::QuestionaireDAO::save(*this);
}

// wire up joining the model to the DAO
bool Epic::DAO::Questionaire::attach(Epic::DAO::Person & person)
{
    return Epic::QuestionaireDAO::attach(*this,person);
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

// wire up finding all people associated with a questionaire
bool Epic::DAO::Questionaire::find_people(std::vector<Epic::DAO::Person> & people)
{
    return Epic::QuestionaireDAO::find_people(*this,people);
}

