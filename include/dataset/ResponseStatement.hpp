#ifndef EPIC_DATABASE_RESPONSE_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_RESPONSE_INSERT_STATEMENT_HPP

#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class ResponseInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            ResponseInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO responses (respondent_id,question_id,value) VALUES (?,?,?) ;")
            {
            }

            void
            bind(sqlite3_int64 respondent, sqlite3_int64 question, const std::string & answer)
            {
               	sqlite3_bind_int64(m_sql, 1, respondent);
               	sqlite3_bind_int64(m_sql, 2, question);
		sqlite3_bind_text(m_sql,3,answer.c_str(),answer.size(),SQLITE_STATIC);
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

#endif /* ndef EPIC_DATABASE_RESPONSE_INSERT_STATEMENT_HPP */
