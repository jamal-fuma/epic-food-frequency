/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
