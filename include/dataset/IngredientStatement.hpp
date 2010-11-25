#ifndef EPIC_DATABASE_INGEDIENT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_INGEDIENT_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class IngredientInsertStatement : public PreparedStatement
        {
        public:
            IngredientInsertStatement() :
                PreparedStatement("INSERT INTO ingredients (meal_id,food,amount,modifier) VALUES (?,?,?,?) ;") { }

            void
            bind(sqlite3_int64 meal_id, const std::string & food_code, double quantity,const std::string & modifier)
            {
                bind_int(1,meal_id);
                bind_text(2,food_code);
                bind_double(3,quantity);
                bind_text(4, modifier);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_INGEDIENT_INSERT_STATEMENT_HPP */
