/* This file is part of the libEpic library */

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

#ifndef EPIC_DATABASE_STATEMENT_HPP
#define EPIC_DATABASE_STATEMENT_HPP

#include "dataset/Database.hpp"
#include "logging/Logger.hpp"

namespace Epic
{
    namespace Database
    {

        class Statement
        {
            public:

            Statement() : m_statement(NULL), m_prepared(false) {}
            Statement(const std::string & sql) : m_statement(NULL), m_prepared(false)
            {
                prepare(sql);
            }
            
            Statement & operator= (const Statement & rhs) {
                if(this != &rhs)
                {
                    if(rhs.m_prepared)
                    {
                        prepare(rhs.sql());
                        return *this;
                    }
                    m_prepared = false;
                    m_statement = NULL;
                }
                return *this;
            }


            void prepare(const std::string & sql)
            {
                if(m_prepared)
                    finalise();

                Epic::Database::prepare(sql,&m_statement);
                m_prepared = true;
            }

            sqlite3_stmt * statement() const
            {
                return m_statement;
            }

            void
            reset()
            {
                sqlite3_reset(m_statement);
	        sqlite3_clear_bindings(m_statement);
            }

            int step()
            {
                return sqlite3_step(m_statement);
            }

            int
            bind_int(sqlite3_int64 row, int value)
            {
                return sqlite3_bind_int(m_statement,row,value);
            }

            int
            bind_int64(sqlite3_int64 row, sqlite3_int64 value)
            {
                return sqlite3_bind_int64(m_statement,row,value);
            }

            int
            bind_text(sqlite3_int64 row, const std::string & value)
            {
                return sqlite3_bind_text(m_statement,
                        row,value.c_str(),value.size(),SQLITE_STATIC);
            }

            int
            bind_double(sqlite3_int64 row, double value)
            {
		return sqlite3_bind_double(m_statement,row,value);
            }
            
            int
            bind_null(sqlite3_int64 row)
            {
		return sqlite3_bind_null(m_statement,row);
            }

            std::string
            column_text(sqlite3_int64 row)
            {
                const char *s = reinterpret_cast<const char *>(sqlite3_column_text(m_statement,row) );
                if(!s)
                    return std::string("NULL");

                return std::string(s);
            }

            sqlite3_int64
            column_int64(sqlite3_int64 row)
            {
                return sqlite3_column_int64(m_statement,row);
            }

            int
            column_int(sqlite3_int64 row)
            {
                return sqlite3_column_int64(m_statement,row);
            }

            double
            column_double(sqlite3_int64 row)
            {
                return sqlite3_column_double(m_statement,row);
            }

            int
            column_count()
            {
                return sqlite3_column_count(m_statement);
            }

            std::string
            sql() const {
                if(m_statement)
                    return sqlite3_sql(m_statement);
                return "";
            }

            ~Statement()
            {
                finalise();
            }

        private:

            void finalise()
            {
                if(m_statement)
                {
                    sqlite3_finalize(m_statement);
                }
                m_statement = NULL;
                m_prepared = false;
            }

            bool m_prepared;
            sqlite3_stmt *m_statement;
        };

        class PreparedStatement
        {
            public:
            PreparedStatement(const std::string & sql) :
                m_bound(false),
                m_sql(sql),
                m_txt(sql)
            {
                /* skip the binding logic if there are no paramters to bind */
                m_bindable = (sql.find('?') != std::string::npos);
                m_bound = (!m_bindable);
            }
            
            PreparedStatement() :
                m_bound(false),
                m_bindable(false),
                m_sql(),
                m_txt("")
            {
            }
        
            PreparedStatement & operator= (const PreparedStatement & rhs) {
                if(this != &rhs)
                {
                    m_bound     = rhs.m_bound;
                    m_bindable  = rhs.m_bindable;
                    m_txt       = rhs.m_txt;
                    m_sql       = rhs.m_sql;
                }
                return *this;
            }

            // throws on error
            void prepare(const std::string & sql)
            {
                /* skip the binding logic if there are no paramters to bind */
                m_bindable = (sql.find('?') != std::string::npos);
                m_bound = (!m_bindable);
                m_sql.prepare(sql);
            }

            template<class T> void
            bind_statement(T & binder)
            {
                m_bound = binder.bind(m_sql);
            }

            // binders
            int
            bind_int(sqlite3_int64 row, int value)
            {
                int rc = m_sql.bind_int(row,value);
                if(SQLITE_OK == rc)
                    m_bound = true;
                return rc;
            }

            sqlite3_int64
            bind_int64(sqlite3_int64 row, sqlite3_int64 value)
            {
                int rc = m_sql.bind_int64(row,value);
                if(SQLITE_OK == rc)
                    m_bound = true;
                return rc;
            }

            int
            bind_text(sqlite3_int64 row, const std::string & value)
            {
                int rc = m_sql.bind_text(row,value);
                if(SQLITE_OK == rc)
                    m_bound = true;
                return rc;
            }

            int
            bind_double(sqlite3_int64 row, double value)
            {
                int rc = m_sql.bind_double(row,value);
                if(SQLITE_OK == rc)
                    m_bound = true;

                return rc;
            }

            int
            bind_null(sqlite3_int64 row)
            {
                int rc = m_sql.bind_null(row);
                if(SQLITE_OK == rc)
                    m_bound = true;

                return rc;
            }

            // column readers

            std::string
            column_text(sqlite3_int64 row)
            {
                return m_sql.column_text(row);
            }

            sqlite3_int64
            column_int64(sqlite3_int64 row)
            {
                return m_sql.column_int64(row);
            }

            int
            column_int(sqlite3_int64 row)
            {
                return m_sql.column_int(row);
            }

            double
            column_double(sqlite3_int64 row)
            {
                return m_sql.column_double(row);
            }

            int
            step()
            {
                if(!m_bound)
                {
                    throw std::runtime_error("Need to bind statement prior to exec");
                }

                return m_sql.step();
            }

            void
            reset()
            {
                if(m_bound)
                {
                    m_sql.reset();
                    m_bound = (!m_bindable);
                }
            }
        private:
            bool      m_bindable; // are there arguments to substitute into statement
            bool      m_bound;    // have all arguments been bound to statement
            Statement m_sql;
            std::string m_txt;
        };

    } // Epic::Database

} // Epic


#endif /*ndef EPIC_DATABASE_STATEMENT_HPP */
