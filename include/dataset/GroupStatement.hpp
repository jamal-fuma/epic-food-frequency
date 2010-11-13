#ifndef EPIC_DATABASE_GROUP_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_GROUP_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class GroupInsertStatement : public PreparedStatement
        {
        public:
            GroupInsertStatement() :
                PreparedStatement("INSERT INTO groups (code,description) VALUES (?,?) ;") { }

            void
            bind(sqlite3_int64 code,const std::string & group )
            {
                bind_int(1,code);
                bind_text(2,group);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_GROUP_INSERT_STATEMENT_HPP */
