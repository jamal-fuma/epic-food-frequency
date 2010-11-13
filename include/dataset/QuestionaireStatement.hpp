#ifndef EPIC_DATABASE_QUESTIONAIRE_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_QUESTIONAIRE_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class QuestionaireInsertStatement : public PreparedStatement
        {
        public:
            QuestionaireInsertStatement() :
                PreparedStatement("INSERT INTO questionaires (code,filename) VALUES (?,?) ;")
            {
            }

            void
            bind(const std::string & code, const std::string & filename)
            {
                bind_text(1,code);
                bind_text(2,filename);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_QUESTIONAIRE_INSERT_STATEMENT_HPP */
