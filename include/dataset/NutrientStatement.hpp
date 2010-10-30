
#ifndef EPIC_DATABASE_NUTRIENT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_NUTRIENT_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class NutrientInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            NutrientInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO nutrients (code,description,units) VALUES (?,?,?) ;")
            {
            }

            void
            bind(long code, const std::string & description, const std::string & units )
            {
		sqlite3_bind_int(m_sql,1,code);
		sqlite3_bind_text(m_sql,2,description.c_str(),description.size(),SQLITE_STATIC);
		sqlite3_bind_text(m_sql,3,units.c_str(),units.size(),SQLITE_STATIC);
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

#endif /* ndef EPIC_DATABASE_NUTRIENT_INSERT_STATEMENT_HPP */
