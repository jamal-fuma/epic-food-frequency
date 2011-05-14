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
                
                std::string last_error();
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
        
        std::string last_error();

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
