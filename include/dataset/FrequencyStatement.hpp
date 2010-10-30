#ifndef EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class FrequencyInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            FrequencyInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO frequencies (code,amount) VALUES (?,?) ;")
            {
            }

            void
            bind(long frequency, double multiplier)
            {
		sqlite3_bind_int(m_sql,1,frequency);
		sqlite3_bind_double(m_sql,2,multiplier);
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

#endif /* ndef EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP */
