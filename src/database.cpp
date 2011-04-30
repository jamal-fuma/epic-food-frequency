/* This file is part of the libEpic client */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dataset/Database.hpp"

#include "libUtility.h"
#include "logging/Logger.hpp"
#include "config/Global.hpp"
#include "config/Resource.hpp"

// models for populating tables
#include "dao/Nutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/MealFood.hpp"
#include "dao/Weight.hpp"
#include "dao/Frequency.hpp"
#include "dao/Portion.hpp"
#include "dao/Cereal.hpp"
#include "dao/Milk.hpp"

extern "C" {
    void log_queries(void * userdata, char const * psql);

    void log_queries(void * userdata, char const * psql)
    {
        if(psql)
        {
            Epic::Logging::Trace().log() << std::string(psql) ;
        }
    }
}
    
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
        Epic::Logging::Error().log() << "Couldn't connect to database: [" << filename << "]";
        throw std::runtime_error("Could not connect to database");
    }
#if(0)
    // set up query tracing
    sqlite3_trace(m_db, log_queries, NULL);
#endif
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
        Epic::Logging::Error().log() << "Couldn't execute database statement: [" << sql <<"]\n" <<
        "SQL library error was " << errmsg ;

        /* sqlite uses it's own  pool so need to clean up it's errors */
        sqlite3_free(errmsg);
        errmsg = NULL;

        throw std::runtime_error(sql.c_str());
    }
}

void Epic::Database::DBConnection::prepare( const std::string & sql, sqlite3_stmt **p_statement)
{
    if(sqlite3_prepare_v2(m_db, sql.c_str(), sql.size(), p_statement, NULL))
    {
        Epic::Logging::Error().log() << "Couldn't prepare database statement: [" << sql <<"]";
        throw std::runtime_error(sql.c_str());
    }
}

std::string Epic::Database::DBConnection::last_error()
{
    return  std::string(sqlite3_errmsg(m_db));
}
bool Epic::Database::connect()
{
    try
    {
        // find name of database from config file
        std::string db_name;
        if(!Epic::Config::find("database",db_name))
        {
            Epic::Logging::Error().log() << "Config file lacks value for 'database'";
            return false;
        }

        // find name of database schema from config file
        std::ostringstream ss;
        std::string filename;
        if(!Epic::Config::find("schema",filename))
        {
            Epic::Logging::Error().log() << "Config file lacks value for 'schema'";
            return false;
        }
        

        // load database schema ready to execute
        std::string schema;
        if(!Epic::Config::Resource::load(filename,schema))
        {
            Epic::Logging::Error().log() << "Unable to load database schema from file '" << filename << "'" ;
            return false;
        }

        // connect to db
        Epic::Pattern::Singleton< DBConnection >::instance().connect(db_name);

        // insert schema on first use of database
        if( Epic::Pattern::Singleton< DBConnection >::instance().created())
        {
            Epic::Database::execute(schema);

            // load lookup data on first use of database
            if(!Epic::DAO::Food::load())
                return false;

            if(!Epic::DAO::Meal::load())
                return false;

            if(!Epic::DAO::Nutrient::load())
                return false;

            if(!Epic::DAO::FoodNutrient::load())
                return false;

            if(!Epic::DAO::MealFood::load())
                return false;

            if(!Epic::DAO::Weight::load())
                return false;

            if(!Epic::DAO::Portion::load())
                return false;

            if(!Epic::DAO::Frequency::load())
                return false;

            if(!Epic::DAO::Cereal::load())
                return false;

            if(!Epic::DAO::Milk::load())
                return false;
        }

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
        Epic::Logging::Error().log() << "Couldn't prepare database statement: " << "statement pointer is NULL" ;
        throw std::runtime_error(sql.c_str());
    }

    Epic::Pattern::Singleton< DBConnection >::instance().prepare(sql,p_statement);
}

std::string Epic::Database::last_error()
{
    return  Epic::Pattern::Singleton< DBConnection >::instance().last_error();
}
