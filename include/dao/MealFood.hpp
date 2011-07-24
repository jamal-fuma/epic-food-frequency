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

#ifndef EPIC_DAO_MEAL_FOOD_HPP
#define EPIC_DAO_MEAL_FOOD_HPP


#include <vector>
#include <string>
#include <iostream>
#include "libsqlite/sqlite3.h"

namespace Epic
{
    namespace DAO
    {
        class MealFood
        {
            public:
                enum Types { Simple = 0, VisibleFat, BakingFat, Cereal, FryingFat };

                MealFood() : 
                    m_valid(false), 
                    m_meal_id(0),
                    m_food_id(0), 
                    m_modifier(MealFood::Simple), 
                    m_amount(0.0) {}

                static MealFood find_by_id(sqlite3_int64 id) ;
                static bool load();
                static bool load(const std::string & filename);
                bool save();

                sqlite3_int64 get_id() const {
                    return m_id;
                }

                sqlite3_int64 get_food_id() const {
                    return m_food_id;
                }
                
                int get_modifier() const {
                    return m_modifier;
                }

                sqlite3_int64 get_meal_id() const {
                    return m_meal_id;
                }

                double get_amount() const {
                    return m_amount;
                }
                
                double get_scaled_amount(double scalar ) const {
                    return (m_amount * scalar);
                }
               
                
                void set_id(sqlite3_int64 id) {
                    m_id = id;
                }

                void set_modifier(const std::string & modifier) {
                    if(modifier == "a")
                        m_modifier = MealFood::VisibleFat;

                    else if(modifier == "b")
                        m_modifier = MealFood::BakingFat;
 
                    else if(modifier == "c")
                        m_modifier = MealFood::Cereal;
                
                    else if(modifier == "d")
                        m_modifier = MealFood::FryingFat;
                    else
                        m_modifier = MealFood::Simple;
                }

                void set_modifier(int modifier) {
                    m_modifier = modifier;
                }
                
                void set_food_id(sqlite3_int64 food_id) {
                    m_food_id = food_id;
                }
                
                void set_meal_id(sqlite3_int64 meal_id) {
                    m_meal_id = meal_id;
                }

               
                inline bool visible_fat_field() {
                    return (m_modifier == MealFood::VisibleFat) ;
                }
                
                inline bool baking_fat_field() {
                    return (m_modifier == MealFood::BakingFat) ;
                }
                
                inline bool cerial_field() {
                    return (m_modifier == MealFood::Cereal) ;
                }

                inline bool frying_fat_field() {
                    return (m_modifier == MealFood::FryingFat) ;
                }
                               
                inline bool simple_field() {
                    return (m_modifier == MealFood::Simple) ;
                }

                void set_amount(double amount) {
                    m_amount = amount;
                }
                
                bool validate() {
                    m_valid = (m_meal_id > 0 && m_amount >0) ;
                    if(!m_valid)
                        return false;

                    if( m_food_id > 0) 
                        m_valid = (simple_field() || visible_fat_field());
                    
                    else if( 0 == m_food_id )
                        m_valid = ((frying_fat_field() || baking_fat_field() || cerial_field()) );
                            
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const MealFood & meal_food) {
                    rhs << "MealFood: ["    << meal_food.get_meal_id()               << ","
                                                << meal_food.get_food_id()           << "," 
                                                << meal_food.get_amount()            <<  ","
                                                << meal_food.get_modifier()
                                      << "]" << std::endl;
                    return rhs;
                }
            private:
                sqlite3_int64 m_id;
                sqlite3_int64 m_meal_id;
                sqlite3_int64 m_food_id;
                int           m_modifier;
                double        m_amount;
                bool          m_valid;
        };

    } // DAO namespace

} // Epic namespace


#endif /* ndef  EPIC_DAO_MEAL_FOOD_HPP */

