#ifndef EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class FoodInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            FoodInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO foods (code,nutrient,amount) VALUES (?,?,?) ;")
            {
            }

            void
            bind(const std::string & food_code, long nutrient_code, double quantity)
            {
		sqlite3_bind_text(m_sql,1,food_code.c_str(),food_code.size(),SQLITE_STATIC);
		sqlite3_bind_int(m_sql,2,nutrient_code);
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

#endif /* ndef EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP */
