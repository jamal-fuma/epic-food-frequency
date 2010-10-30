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

    } // Epic::Database
    
} // Epic


#endif /*ndef EPIC_DATABASE_STATEMENT_HPP */
