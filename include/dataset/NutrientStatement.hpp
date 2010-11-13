
#ifndef EPIC_DATABASE_NUTRIENT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_NUTRIENT_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class NutrientInsertStatement : public PreparedStatement
        {
        public:
            NutrientInsertStatement() :
                PreparedStatement("INSERT INTO nutrients (code,description,units) VALUES (?,?,?) ;")
            {
            }

            void
            bind(long code, const std::string & description, const std::string & units )
            {
		bind_int(1,code);
	        bind_text(2,description);
	        bind_text(3,units);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_NUTRIENT_INSERT_STATEMENT_HPP */
