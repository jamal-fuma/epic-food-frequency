#ifndef EPIC_DATABASE_TABLE_QUERY_STATEMENT_HPP
#define EPIC_DATABASE_TABLE_QUERY_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class TableQueryStatement
        {
            Statement m_sql;
        public:
            TableQueryStatement(DBConnection & db) : 
	        m_sql(db,"SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name;");
            {
            }
            
            int
            exec(std::vector<std::string> & tables)
            {
                int rc = SQLITE_DONE;
                while( SQLITE_ROW == (rc = sqlite3_step(m_sql)))
                {        
                    tables.push_back(sqlite3_column_text(m_sql,1));
                }
                m_sql.reset();
                return rc;
            }
        };


    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_TABLE_QUERY_STATEMENT_HPP */
