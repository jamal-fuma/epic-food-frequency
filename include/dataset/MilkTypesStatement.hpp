#ifndef EPIC_DATABASE_MILK_TYPES_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_MILK_TYPES_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        namespace Model
        {
            struct Milk
            {
                sqlite3_int64 m_id;
                std::string   m_food_code;
                std::string   m_desc;
                
                Milk( sqlite3_int64 id, const std::string & food_code, const std::string & desc)
                    : m_id(id), m_food_code(food_code), m_desc(desc) {}

                bool
                bind(Database::Statement & sql) const
                {
                    sql.bind_int(1,m_id);
                    sql.bind_text(2,m_food_code);
                    sql.bind_text(3,m_desc);
                    return true;
                }

            };
        } // Database::Model

        class MilkTypeInsertStatement : public PreparedStatement
        {
            public:
            MilkTypeInsertStatement(DBConnection & db) 
                : PreparedStatement(db,
                        "INSERT INTO milk_types (code,food,description) VALUES (?,?,?) ;"){ }

            void bind(  sqlite3_int64 code, 
                        const std::string & food_code,
                        const std::string & desc)
            {
                Model::Milk milk(code,food_code,desc);
                bind_statement(milk);
            }
        };

    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_MILK_TYPES_INSERT_STATEMENT_HPP */

