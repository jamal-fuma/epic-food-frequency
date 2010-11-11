#include "dataset/Database.hpp"

#include "libUtility.h"
#include "logging/Logger.hpp"
#include "config/Global.hpp"
#include "config/Resource.hpp"

void Epic::Database::DBConnection::connect(const std::string & filename)
{
    if(m_connected)
    {
        disconnect();
        m_connected = false;
    }

    m_created = false;
    int rc = utility_file_exists(filename.c_str());
    if(util_eError == rc || (0 == rc))
    {
        m_created = true;
    }

    if(SQLITE_OK != sqlite3_open(filename.c_str(), &m_db))
    {
        sqlite3_close(m_db);
        m_db = NULL;
        throw std::runtime_error("Could not connect to database");
    }
    m_connected = true;
}

void Epic::Database::DBConnection::disconnect()
{
    if(m_db)
    {
        sqlite3_close(m_db);
    }
    m_db = NULL;
    m_connected = false;
}


void Epic::Database::DBConnection::exec(const std::string & sql)
{
    char *errmsg=NULL;
    if(SQLITE_OK != sqlite3_exec(m_db,sql.c_str(),NULL,0,&errmsg))
    {
        std::ostringstream ss;
        ss << "Couldn't execute database statement: " << sql << std::endl;
        ss <<  "SQL library error was " << errmsg << std::endl;

        /* sqlite uses it's own  pool so need to clean up it's errors */
        sqlite3_free(errmsg);
        errmsg = NULL;

        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str().c_str());
    }
}

void Epic::Database::DBConnection::prepare( const std::string & sql, sqlite3_stmt **p_statement)
{
    if(sqlite3_prepare_v2(m_db, sql.c_str(), sql.size(), p_statement, NULL))
    {
        std::ostringstream ss;
        ss << "Couldn't prepare database statement: " << sql << std::endl;

        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str().c_str());
    }
}


bool Epic::Database::connect()
{
    try
    {
        // find name of database from config file
        std::string db_name;
        if(!Epic::Config::find("database",db_name))
        {
            Epic::Logging::error("Config file lacks value for 'database'\n");
            return false;
        }

        // find name of database schema from config file
        std::ostringstream ss;
        std::string filename;
        if(!Epic::Config::find("schema",filename))
            return false;

        // load database schema ready to execute
        std::string schema;
        if(!Epic::Config::Resource::load(filename,schema))
        {
            ss << "Unable to load database schema from file '" << filename << "'\n" ;
            Epic::Logging::error(ss.str());
            return false;
        }

        // connect to db
        Epic::Pattern::Singleton< DBConnection >::instance().connect(db_name);

        // insert schema on first use of database
        if( Epic::Pattern::Singleton< DBConnection >::instance().created())
            Epic::Database::execute(schema);

        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Database::created()
{
    return Epic::Pattern::Singleton< DBConnection >::instance().created();
}

int Epic::Database::execute(const std::string & sql)
{
    Epic::Pattern::Singleton< DBConnection >::instance().exec(sql);
    return SQLITE_OK;
}

sqlite3_int64 Epic::Database::last_insert_id()
{
    return sqlite3_last_insert_rowid(
            Epic::Pattern::Singleton< DBConnection >::instance().database());
}

void Epic::Database::prepare( const std::string & sql,sqlite3_stmt **p_statement)
{
    if(!p_statement)
    {
        std::ostringstream ss;
        ss << "Couldn't prepare database statement: " << "statement pointer is NULL" << std::endl;
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str().c_str());
    }

    Epic::Pattern::Singleton< DBConnection >::instance().prepare(sql,p_statement);
}


