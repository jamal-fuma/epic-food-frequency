#ifndef EPIC_DATABASE_QUESTION_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_QUESTION_INSERT_STATEMENT_HPP

#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class QuestionInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            QuestionInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO questions (question_type_id,description) VALUES (?,?) ;")
            {
            }

            void
            bind(long type, const std::string & title)
            {
               	sqlite3_bind_int(m_sql, 1, type);
		sqlite3_bind_text(m_sql,2,title.c_str(),title.size(),SQLITE_STATIC);
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

#endif /* ndef EPIC_DATABASE_QUESTION_INSERT_STATEMENT_HPP */
