#ifndef EPIC_IMPORT_INGREDIENT_HPP
#define EPIC_IMPORT_INGREDIENT_HPP

#include "import/Import.hpp"
#include "dataset/IngredientStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Ingredient
        {
            static const size_t fields = 3;
        };

        struct IngredientData
        {
            Database::Transaction                   m_transaction;
            Database::IngredientInsertStatement     m_statement;

            IngredientData() { }

            bool
            operator()(std::string & error_message)
            {
                std::cerr << error_message << std::endl;
                return false;
            }

            // header
            bool operator()(str_vector_t & v)
            {
                str_vector_t expected;
                expected.push_back("LINE");
                expected.push_back("FOOD_CODE");
                expected.push_back("PORTION");
                return Epic::Import::DBModel::same_header("ingredients",expected,v);
            }

            // data lines
            bool operator()(size_t line, str_vector_t & v)
            {
                if(v.size() ==  Ingredient::fields)
                {
                    // frequency code
                    Conversion::IntegerString meal_code(v[0]);
                    long code = meal_code;

                    // quantity
                    Conversion::IntegerString portion_quantity(v[2]);
                    long quantity = portion_quantity;

                    m_statement.bind(code,v[1],quantity);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in ingredients import file: ";
                    std::cerr << "skipping line :" << line;
                }
                return true;
            }
            bool commit()
            {
                return m_transaction.commit();
            }
        };
    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_INGREDIENT_HPP */
