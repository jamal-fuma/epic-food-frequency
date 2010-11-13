#ifndef EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class FoodInsertStatement : public PreparedStatement
        {
        public:
            FoodInsertStatement() :
                PreparedStatement("INSERT INTO foods (code,nutrient,amount) VALUES (?,?,?) ;")
            {
            }

            void
            bind(const std::string & food_code, sqlite3_int64 nutrient_code, double quantity)
            {
	        bind_text(1,food_code);
	        bind_int(2,nutrient_code);
                bind_double(3,quantity);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP */
