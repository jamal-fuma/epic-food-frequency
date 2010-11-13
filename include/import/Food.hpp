#ifndef EPIC_IMPORT_FOODS_HPP
#define EPIC_IMPORT_FOODS_HPP

#include "import/Import.hpp"
#include "dataset/FoodStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Food
        {
            static const size_t fields = 3;
        };

        struct FoodData
        {
            Database::Transaction               m_transaction;
            Database::FoodInsertStatement       m_statement;
            std::map < std::string, sqlite3_int64 > m_code_to_id;

            FoodData()
            {

            }

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
                expected.push_back("FOOD_CODE");
                expected.push_back("NUTRIENT");
                expected.push_back("QUANTITY");
                return Epic::Import::DBModel::same_header("foods",expected,v);
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Food::fields)
                {
                    // nutrient code
                    Conversion::IntegerString nutrient_id(v[1]);
                    long id = nutrient_id;

                    // quantity
                    Conversion::NutrientQuantity nutrient_quantity(v[2]);
                    double quantity = nutrient_quantity;

                    // food code is just a string
                    m_statement.bind(v[0],id,quantity);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in foods import file: ";
                    std::cerr << "skipping line :" << line;
                }
                return true;
            }

            bool
            commit()
            {
                return m_transaction.commit();
            }
        };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_FOODS_HPP */
