#ifndef EPIC_DATABASE_HPP
#define EPIC_DATABASE_HPP

#include "libUtility.h"
#include "Utility_internal.h"
#include <string>
#include <stdexcept>
namespace Epic
{
    namespace Database
    {
        class DBConnection
        {
            public:
                DBConnection(const std::string & filename)
                {
                    if(SQLITE_OK != sqlite3_open(filename.c_str(), &m_db))
                    {
                        sqlite3_close(m_db);
                        m_db = NULL;
                        throw std::runtime_error("Could not connect to database");
                    }
                }

                operator sqlite3* () const
                {
                    return m_db;
                }

                ~DBConnection()
                {
                    if(m_db)
                    {
                        sqlite3_close(m_db);
                    }
                    m_db = NULL;
                }
            private:
                sqlite3 *m_db;
        };

        class Transaction
        {
            DBConnection & m_db;
            bool           m_commited;
            public:
            Transaction(DBConnection & db) : m_db(db),m_commited(false)
            {
	        if(SQLITE_OK != sqlite3_exec(m_db,"BEGIN TRANSACTION;",NULL,0,NULL))
                    throw std::runtime_error("Couldn't begin transaction");
            }
            
            bool
            commit()
            {
	        m_commited = (SQLITE_OK == sqlite3_exec(m_db,"COMMIT;",NULL,0,NULL));
	        return (m_commited);
            }

            ~Transaction()
            {
                if(!m_commited)
		    sqlite3_exec(m_db,"ROLLBACK;",NULL,0,NULL);
            }

        };
    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_HPP */
