#ifndef EPIC_DATABASE_STATEMENT_HPP
#define EPIC_DATABASE_STATEMENT_HPP

#include "../include/dataset/Database.hpp"

namespace Epic
{
    namespace Database
    {
 
        class Statement
        {
            public:

            Statement(DBConnection & db,const std::string & sql) :m_db(db), m_statement(NULL)
            {
		if(sqlite3_prepare_v2(db, sql.c_str(), strlen(sql.c_str()), &m_statement, NULL))
                {
                    throw std::runtime_error("Could not prepare database statement '" + sql + "'");
                }
            }

            operator sqlite3_stmt *() const
            {
                return m_statement;
            }

            void
            reset()
            {
                sqlite3_reset(m_statement);
	        sqlite3_clear_bindings(m_statement);
            }

            int
            bind_int(sqlite3_int64 row, sqlite3_int64 value)
            {
                return sqlite3_bind_int(m_statement,row,value);
            }
            
            int
            bind_text(sqlite3_int64 row, const std::string & value)
            {
                return sqlite3_bind_text(m_statement,row,value.c_str(),value.size(),SQLITE_STATIC);
            }

            ~Statement()
            {
                if(m_statement)
                {
                    sqlite3_finalize(m_statement);
                }
            }
        private:
            DBConnection & m_db;
            sqlite3_stmt *m_statement;
        };
        
        class PreparedStatement
        {
            bool      m_bound;
            Statement m_sql;

            public:
            PreparedStatement(DBConnection & db,const std::string & sql) : 
                m_bound(false),
                m_sql(db,sql)
            {
            }
            
            template<class T> void
            bind_statement(T & binder)
            {
                m_bound = binder.bind(m_sql);
            }

            bool
            step()
            {
                if(!m_bound)
                {
                    throw std::runtime_error("Need to bind statement prior to exec");
                }

                return (SQLITE_DONE == sqlite3_step(m_sql));
            }

            void
            reset()
            {
                if(m_bound)
                {
                    m_sql.reset();
                    m_bound = false;
                }
            }
        };

    } // Epic::Database
    
} // Epic


#endif /*ndef EPIC_DATABASE_STATEMENT_HPP */
