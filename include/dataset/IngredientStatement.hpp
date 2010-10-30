#ifndef EPIC_DATABASE_INGEDIENT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_INGEDIENT_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class IngredientInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            IngredientInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO ingredients (meal_id,food,amount) VALUES (?,?,?) ;")
            {
            }

            void
            bind(long meal_id, const std::string & food_code, double quantity)
            {
		sqlite3_bind_int(m_sql,1,meal_id);
		sqlite3_bind_text(m_sql,2,food_code.c_str(),food_code.size(),SQLITE_STATIC);
		sqlite3_bind_double(m_sql,3,quantity);
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

#endif /* ndef EPIC_DATABASE_INGEDIENT_INSERT_STATEMENT_HPP */
