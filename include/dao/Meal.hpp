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

#ifndef EPIC_DAO_MEAL_HPP
#define EPIC_DAO_MEAL_HPP

#include "Epic_lib.hpp"
#include <vector>
#include "dao/MealFood.hpp"

namespace Epic
{
    namespace DAO
    {
        class Meal 
        {
            public:
                Meal() : m_id(0), m_valid(false) {}

                explicit Meal(const std::string & name, const std::string & description) :
                    m_valid(false), 
                    m_id(0),
                    m_name(name),
                    m_description(description) {}

                static Meal find_by_name(const std::string & name) ;
                static Meal find_by_id(sqlite3_int64 id) ;

                // find all meals with visible fat
                static bool find_all_with_visible_fat(std::vector<Epic::DAO::Meal> & meals);

                // find all meals with baking fat
                static bool find_all_with_baking_fat(std::vector<Epic::DAO::Meal> & meals);

                // find all meals with cereal process
                static bool find_all_with_cereal(std::vector<Epic::DAO::Meal> & meals);

                // find all meals with frying fat
                static bool find_all_with_frying_fat(std::vector<Epic::DAO::Meal> & meals);

                // find all meals with simple processing
                static bool find_all_without_modifier(std::vector<Epic::DAO::Meal> & meals);

                static bool find_all(std::vector<Epic::DAO::Meal> & meals);

                static bool load();
                static bool load(const std::string & filename);
                bool save();

                bool find_all_foods(std::vector<Epic::DAO::MealFood> & meal_foods) const; 

                // accessors
                sqlite3_int64 get_id() const { 
                    return m_id; 
                }

                std::string get_name() const { 
                    return m_name; 
                }

                std::string get_description() const { 
                    return m_description; 
                }

                bool name_empty() const { 
                    return m_name.empty() ; 
                }
                
                bool description_empty() const { 
                    return m_description.empty() ; 
                }

                // mutators

                void set_id(sqlite3_int64 id) { 
                    m_id = id; 
                }
 
                void set_name(const std::string & name) { 
                    m_name = name; 
                }

                void set_description(const std::string & description) { 
                    m_description = description; 
                }

                bool validate() {
                    m_valid = ( (m_id > 0) && (!name_empty() && !description_empty()));
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Meal & meal) {
                    rhs << "Meal: [" << meal.get_id()               << ",'" 
                                         << meal.get_name()         << "','"
                                         << meal.get_description()  << "'"
                                  << "]";
                    return rhs;
                }

            private:
                sqlite3_int64   m_id;
                std::string     m_name;
                std::string     m_description;
                bool            m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_MEAL_HPP */
