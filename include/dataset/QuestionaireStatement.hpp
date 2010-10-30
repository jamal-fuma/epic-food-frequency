#ifndef EPIC_DATABASE_QUESTIONAIRE_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_QUESTIONAIRE_INSERT_STATEMENT_HPP

#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class QuestionaireInsertStatement
        {
            bool      m_bound;
            Statement m_sql;

        public:
            QuestionaireInsertStatement(DBConnection & db) : 
                m_bound(false),
                m_sql(db,"INSERT INTO questionaires (code,filename) VALUES (?,?) ;")
            {
            }

            void
            bind(const std::string & code, const std::string & filename)
            {
		sqlite3_bind_text(m_sql,1,code.c_str(),code.size(),SQLITE_STATIC);
		sqlite3_bind_text(m_sql,2,filename.c_str(),filename.size(),SQLITE_STATIC);
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

#endif /* ndef EPIC_DATABASE_QUESTIONAIRE_INSERT_STATEMENT_HPP */
