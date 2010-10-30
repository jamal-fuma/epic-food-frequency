#ifndef EPIC_DATABASE_MEAL_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_MEAL_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class MealInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            MealInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO meals (line,name,description) VALUES (?,?,?) ;")
            {
            }

            void
            bind(long line, const std::string & name, const std::string & desc)
            {
               	sqlite3_bind_int(m_sql, 1, line);
		sqlite3_bind_text(m_sql,2,name.c_str(),name.size(),SQLITE_STATIC);
		sqlite3_bind_text(m_sql,3,desc.c_str(),desc.size(),SQLITE_STATIC);
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

#endif /* ndef EPIC_DATABASE_MEAL_INSERT_STATEMENT_HPP */
