#ifndef EPIC_DATABASE_RESPONDENT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_RESPONDENT_INSERT_STATEMENT_HPP

#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class RespondentInsertStatement : public PreparedStatement
        {
        public:
            RespondentInsertStatement() : 
                PreparedStatement(
                "INSERT INTO respondents (questionaire_id,reference) "
                "VALUES ( (select max(id) from questionaires),?) ;"
                )
            {
            }

            void
            bind(const std::string & reference)
            {
		bind_text(1,reference);
            }
        };

    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_RESPONDENT_INSERT_STATEMENT_HPP */
