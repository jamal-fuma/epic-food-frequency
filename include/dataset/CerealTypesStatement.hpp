#ifndef EPIC_DATABASE_CEREAL_TYPES_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_CEREAL_TYPES_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
    
        class CerealTypeInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            CerealTypeInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO cereal_types (code,food,description,weight_id) VALUES (?,?,?,?) ;")
            {
            }
            void
            bind(long code, const std::string & food_code, const std::string & desc,long weight_id)
            {
		sqlite3_bind_int(m_sql,1,code);
		sqlite3_bind_text(m_sql,2,food_code.c_str(),food_code.size(),SQLITE_STATIC);
		sqlite3_bind_text(m_sql,3,desc.c_str(),desc.size(),SQLITE_STATIC);
		sqlite3_bind_int(m_sql,4,weight_id);
                m_bound = true;
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

#endif /* ndef EPIC_DATABASE_CEREAL_TYPES_INSERT_STATEMENT_HPP */
