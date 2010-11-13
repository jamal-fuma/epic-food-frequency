#ifndef EPIC_DATABASE_MILK_TYPES_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_MILK_TYPES_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class MilkTypeInsertStatement : public PreparedStatement
        {
            public:
            MilkTypeInsertStatement()
                : PreparedStatement(
                        "INSERT INTO milk_types (code,food,description) VALUES (?,?,?) ;"){ }

            void bind(  sqlite3_int64 code,
                        const std::string & food_code,
                        const std::string & desc)
            {
                bind_int(1,code);
                bind_text(2,food_code);
                bind_text(3,desc);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_MILK_TYPES_INSERT_STATEMENT_HPP */

