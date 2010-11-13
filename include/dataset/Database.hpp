#ifndef EPIC_DATABASE_HPP
#define EPIC_DATABASE_HPP

#include "sqlite3.h"
#include <string>
#include <stdexcept>

namespace Epic
{
    namespace Database
    {
        class DBConnection
        {
            public:
                DBConnection() : m_created(false),m_connected(false),m_db(NULL) {}

                void connect(const std::string & filename);
                void disconnect();

                sqlite3* database() const
                {
                    if(m_connected)
                        return m_db;

                    throw std::runtime_error("Database not connected");
                }

                ~DBConnection()
                {
                    disconnect();
                }

                bool created()
                {
                    return m_created;
                }

                void exec( const std::string & sql);

                void prepare(const std::string & sql, sqlite3_stmt **p_statement);

             private:
                sqlite3 *m_db;
                bool m_created;
                bool m_connected;
        };

        // wrappers for the db singleton
        bool connect();
        bool created();
        int execute( const std::string & sql);
        void prepare( const std::string & sql,sqlite3_stmt **p_statement);

        sqlite3_int64 last_insert_id();

        class Transaction
        {
            bool           m_commited;
        public:
            Transaction() : m_commited(false)
            {
                Epic::Database::execute("BEGIN TRANSACTION;");
            }

            bool
            commit()
            {
	        m_commited = (SQLITE_OK == Epic::Database::execute("COMMIT;"));
	        return (m_commited);
            }

            ~Transaction()
            {
                if(!m_commited)
                    Epic::Database::execute("ROLLBACK;");
            }
        };
    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_HPP */
