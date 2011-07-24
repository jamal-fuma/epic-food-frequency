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

#ifndef EPIC_DAO_FOOD_NUTRIENT_HPP
#define EPIC_DAO_FOOD_NUTRIENT_HPP


#include <vector>
#include <string>
#include <iostream>
#include "libsqlite/sqlite3.h"
namespace Epic
{
    namespace DAO
    {
        class FoodNutrient
        {
            public:
                FoodNutrient() : 
                    m_valid(false), 
                    m_food_id(0),
                    m_nutrient_id(0), 
                    m_nutrient_code(0), 
                    m_amount(0.0) {}

                static bool load();
                static bool load(const std::string & filename);
                sqlite3_int64 get_nutrient_id() const {
                    return m_nutrient_id;
                }
                
                int get_nutrient_code() const {
                    return m_nutrient_code;
                }

                sqlite3_int64 get_food_id() const {
                    return m_food_id;
                }

                double get_amount() const {
                    return m_amount;
                }
                
                double get_scaled_amount(double scalar ) const {
                    return ( (m_amount / 100.00) * scalar);
                }
                
                void set_nutrient_code(int nutrient_code) {
                    m_nutrient_code = nutrient_code;
                }
                
                void set_nutrient_id(sqlite3_int64 nutrient_id) {
                    m_nutrient_id = nutrient_id;
                }
                
                void set_food_id(sqlite3_int64 food_id) {
                    m_food_id = food_id;
                }

                void set_amount(double amount) {
                    m_amount = amount;
                }
                
                bool validate() {
                    m_valid = ( (m_food_id > 0) && (m_nutrient_id > 0 ));
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const FoodNutrient & food_nutrient) {
                    rhs << "FoodNutrient: ["    << food_nutrient.get_food_id()               << ","
                                                << food_nutrient.get_nutrient_id()           << "," 
                                                << food_nutrient.get_nutrient_code()         << "," 
                                                << food_nutrient.get_amount()
                                      << "]" << std::endl;
                    return rhs;
                }
            private:
                sqlite3_int64 m_food_id;
                sqlite3_int64 m_nutrient_id;
                int           m_nutrient_code;
                double        m_amount;
                bool          m_valid;
        };

    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_FOOD_NUTRIENT_HPP */

