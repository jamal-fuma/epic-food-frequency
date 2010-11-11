#ifndef EPIC_DATABASE_MEAL_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_MEAL_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class MealInsertStatement : public PreparedStatement
        {
        public:
            MealInsertStatement() : 
                PreparedStatement("INSERT INTO meals (line,name,description) VALUES (?,?,?) ;") { }

            void
            bind(sqlite3_int64 line, const std::string & name, const std::string & desc)
            {
                bind_int(1,line);
                bind_text(2,name);
                bind_text(3,desc);
            }
        };

    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_MEAL_INSERT_STATEMENT_HPP */
