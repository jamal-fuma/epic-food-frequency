#ifndef EPIC_DATABASE_QUESTION_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_QUESTION_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"

namespace Epic
{
    namespace Database
    {
        class QuestionInsertStatement : public PreparedStatement
        {
        public:
            QuestionInsertStatement() :
                PreparedStatement("INSERT INTO questions (question_type_id,description) VALUES (?,?) ;")
            {
            }

            void
            bind(sqlite3_int64 type, const std::string & title)
            {
                bind_int(1,type);
                bind_text(2,title);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_QUESTION_INSERT_STATEMENT_HPP */
