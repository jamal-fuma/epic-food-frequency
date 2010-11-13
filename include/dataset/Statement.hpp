#ifndef EPIC_DATABASE_STATEMENT_HPP
#define EPIC_DATABASE_STATEMENT_HPP

#include "dataset/Database.hpp"

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
            bind_int(sqlite3_int64 row, sqlite3_int64 value)
            {
                return sqlite3_bind_int(m_statement,row,value);
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

            std::string
            column_text(sqlite3_int64 row)
            {
                const char *s = reinterpret_cast<const char *>(sqlite3_column_text(m_statement,row) );
                if(!s)
                    return std::string("NULL");

                return std::string(s);
            }

            sqlite3_int64
            column_int(sqlite3_int64 row)
            {
                return sqlite3_column_int64(m_statement,row);
            }

            int
            column_count()
            {
                return sqlite3_column_count(m_statement);
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
            bool      m_bound;
            Statement m_sql;

            public:
            PreparedStatement(const std::string & sql) :
                m_bound(false),
                m_sql(sql)
            {
            }

            template<class T> void
            bind_statement(T & binder)
            {
                m_bound = binder.bind(m_sql);
            }

            // binders
            int
            bind_int(sqlite3_int64 row, sqlite3_int64 value)
            {
                int rc = m_sql.bind_int(row,value);
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

            // column readers

            std::string
            column_text(sqlite3_int64 row)
            {
                return m_sql.column_text(row);
            }

            sqlite3_int64
            column_int(sqlite3_int64 row)
            {
                return m_sql.column_int(row);
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
                    m_bound = false;
                }
            }
        };

    } // Epic::Database

} // Epic


#endif /*ndef EPIC_DATABASE_STATEMENT_HPP */
