#ifndef EPIC_DATABASE_CEREAL_TYPES_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_CEREAL_TYPES_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {

        class CerealTypeInsertStatement : public PreparedStatement
        {
        public:
            CerealTypeInsertStatement() :
                PreparedStatement("INSERT INTO cereal_types (code,food,description,weight_id) VALUES (?,?,?,?) ;") { }

            void
            bind(sqlite3_int64 code, const std::string & food_code, const std::string & desc,sqlite3_int64 weight_id)
            {
                bind_int(1,code);
                bind_text(2,food_code);
                bind_text(3,desc);
                bind_int(4,weight_id);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_CEREAL_TYPES_INSERT_STATEMENT_HPP */
