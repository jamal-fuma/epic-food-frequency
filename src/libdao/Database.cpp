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

#include "Database.hpp"

#include "libhelper/Logger.hpp"
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

#define DATABASE_SINGLETON (Epic::Pattern::Singleton< DBConnection >::instance())

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

namespace detail
{
    struct apply_query_logging
    {
        apply_query_logging(bool should_apply) :
            m_should_apply(should_apply) 
            {
        
            }
        bool
        operator()(sqlite3 * db)
        {
            if(m_should_apply)
            {
                // set up query tracing
                sqlite3_trace(db, log_queries, NULL);
            }
            return m_should_apply;
        }
        bool m_should_apply;
    };

    struct configured_database_name
    {
        std::string 
        operator()()
        {
            // find name of database from config file
            std::string db_name(":memory:");
            if(!Epic::Config::find("database",db_name))
            {
                Epic::Logging::Error().log() << "Config file lacks value for 'database'";
                db_name = ":memory:";
            }
            return db_name;
        }
    };
    
    struct configured_schema_name
    {
        std::string 
        operator()()
        {
            // find name of database schema from config file
            std::string filename;
            if(!Epic::Config::find("schema",filename))
            {
                Epic::Logging::Error().log() << "Config file lacks value for 'schema'";
                filename = std::string();
            }
            return filename;
        }
    };

    struct configured_schema_data
    {
        std::string 
        operator()()
        {
            try
            {
                // load database schema ready to execute
                std::string schema;
                Epic::Config::Resource::load(configured_schema_name()(),schema);
                return schema;
            }
            catch(...)
            {
                Epic::Logging::Error().log() << "Unable to load database schema for db: '" << configured_database_name()() << "'" ;
                return std::string();
            }
        }
    }; 

    struct apply_configured_schema
    {
        bool
        operator()()
        {
            std::string schema = configured_schema_data()();
            if(schema.empty())
                return false;

            Epic::Database::execute(schema);
            return true;
        }
    };
 
    struct apply_initial_data
    {
        bool
        operator()()
        {
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

            return true;
        }
    };
    
    struct first_use_of_database
    {
        bool
        operator()(const std::string & filename)
        {
            sqlite3 *db = NULL;
            bool is_first_use = (SQLITE_OK == sqlite3_open_v2(filename.c_str(), &db,SQLITE_OPEN_READONLY,NULL));
            sqlite3_close(db);
            return is_first_use;
        }
    };

} // detail namespace

void Epic::Database::DBConnection::connect(const std::string & filename,bool with_logging)
{
    if(m_connected)
    {
        disconnect();
        m_connected = false;
    }
    
    m_created = detail::first_use_of_database()(filename);


    if(SQLITE_OK != sqlite3_open(filename.c_str(), &m_db))
    {
        sqlite3_close(m_db);
        m_db = NULL;

        Epic::Logging::Error().log() 
            << "Couldn't connect to database: ["
            << filename
            << "]";

        throw std::runtime_error("Could not connect to database");
    }

    detail::apply_query_logging cmd(with_logging);
    cmd(m_db);

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
        Epic::Logging::Error().log() 
            << "Couldn't execute database statement: ["
            << sql
            <<"]\n"
            << "SQL library error was "
            << errmsg ;

        /* sqlite uses it's own  pool so need to clean up it's errors */
        sqlite3_free(errmsg);
        errmsg = NULL;

        throw std::runtime_error(sql.c_str());
    }
}

void Epic::Database::DBConnection::prepare( const std::string & sql, sqlite3_stmt **p_statement)
{
    if(sqlite3_prepare_v2(
                m_db,
                sql.c_str(),
                sql.size(),
                p_statement,
                NULL))
    {
        Epic::Logging::Error().log() 
            << "Couldn't prepare database statement: ["
            << sql
            << "]";

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
         // connect to db
        DATABASE_SINGLETON.connect(detail::configured_database_name()());
        
        if( !Epic::Database::created())
        {
            // not first use of db, so we assume db is in a sane state.
            return true;
        }
        
        // insert schema on first use of database
        if(!detail::apply_configured_schema()())
            return false;

        return detail::apply_initial_data()();
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Database::created()
{
    return DATABASE_SINGLETON.created();
}

int Epic::Database::execute(const std::string & sql)
{
    DATABASE_SINGLETON.exec(sql);
    return SQLITE_OK;
}

sqlite3_int64 Epic::Database::last_insert_id()
{
    return sqlite3_last_insert_rowid(
            DATABASE_SINGLETON.database());
}

void Epic::Database::prepare( const std::string & sql,sqlite3_stmt **p_statement)
{
    if(!p_statement)
    {
        Epic::Logging::Error().log() 
            << "Couldn't prepare database statement: statement pointer is NULL" ;

        throw std::runtime_error(sql.c_str());
    }

    DATABASE_SINGLETON.prepare(sql,p_statement);
}

std::string Epic::Database::last_error()
{
    return  DATABASE_SINGLETON.last_error();
}
