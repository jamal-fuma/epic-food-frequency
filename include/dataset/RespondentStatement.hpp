#ifndef EPIC_DATABASE_RESPONDENT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_RESPONDENT_INSERT_STATEMENT_HPP

#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class RespondentInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            RespondentInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO respondents (questionaire_id,reference) VALUES (?,?) ;")
            {
            }

            void
            bind(sqlite3_int64 questionaire, const std::string & reference)
            {
               	sqlite3_bind_int64(m_sql,1, questionaire);
		sqlite3_bind_text(m_sql,2,reference.c_str(),reference.size(),SQLITE_STATIC);
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

#endif /* ndef EPIC_DATABASE_RESPONDENT_INSERT_STATEMENT_HPP */
