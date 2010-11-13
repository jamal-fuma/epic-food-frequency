#ifndef EPIC_DATABASE_RESPONSE_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_RESPONSE_INSERT_STATEMENT_HPP

#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class ResponseInsertStatement : public PreparedStatement
        {
        public:
            ResponseInsertStatement() :
                PreparedStatement("INSERT INTO responses (respondent_id,question_id,value) VALUES (?,?,?) ;")
            {
            }

            void
            bind(sqlite3_int64 respondent, sqlite3_int64 question, const std::string & answer)
            {
               	bind_int(1, respondent);
               	bind_int(2, question);
		bind_text(3,answer);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_RESPONSE_INSERT_STATEMENT_HPP */
