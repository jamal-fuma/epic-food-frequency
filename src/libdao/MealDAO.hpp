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

#ifndef EPIC_DAO_MEAL_DAO_HPP
#define EPIC_DAO_MEAL_DAO_HPP

#include "dao/Meal.hpp"
#include "dao/MealFood.hpp"

#include <vector>
#include "Statement.hpp"
namespace Epic
{
    namespace MealDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO meals (name,description) VALUES (?,?) ;"),

                m_find_by_name("SELECT id,description FROM meals WHERE name = ? ;"),

                m_find_all_without_modifier(
                                  "SELECT meals.id, meals.name, meals.description FROM meals "
                                  "WHERE meals.id "
                                  "NOT IN ( SELECT meal_foods.meal_id "
                                  "FROM meal_foods WHERE modifier in (?,?,?,?) ) ;"),

                m_find_all_by_type("SELECT meals.id, meals.name, meals.description FROM meals INNER JOIN meal_foods ON meal_foods.meal_id = meals.id WHERE meal_foods.modifier = ? ;"),
                m_find_all("SELECT meals.id, meals.name, meals.description FROM meals ;"),

                m_find_by_id("SELECT name,description FROM meals WHERE id = ? ;") { }

            // find a meal given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Meal & meal);

            // find a meal given a name
            bool find_by_name(const std::string & name, Epic::DAO::Meal & meal);

            // find all meals of the given type
            bool find_all_by_type(Epic::DAO::MealFood::Types type, std::vector<Epic::DAO::Meal> & meals);

            // find all meals
            bool find_all(std::vector<Epic::DAO::Meal> & meals);

            // find all meals without modifiers
            bool find_all_without_modifier(std::vector<Epic::DAO::Meal> & meals);

            // save a meal
            bool save(Epic::DAO::Meal & meal);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_name;
            Epic::Database::PreparedStatement m_find_by_id;
            Epic::Database::PreparedStatement m_find_all_by_type;
            Epic::Database::PreparedStatement m_find_all;
            Epic::Database::PreparedStatement m_find_all_without_modifier;
        };

        // find a meal given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Meal & meal);

        // find a meal given a name
        bool find_by_name(const std::string & name, Epic::DAO::Meal & meal);

        // save a meal
        bool save(Epic::DAO::Meal & meal);

        // find all meal of the given type
        bool find_all_by_type(Epic::DAO::MealFood::Types type, std::vector<Epic::DAO::Meal> & meals);

        // find all meals without modifiers
        bool find_all_without_modifier(std::vector<Epic::DAO::Meal> & meals);

        // find all meals
        bool find_all(std::vector<Epic::DAO::Meal> & meals);

    } // MealDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_MEAL_DAO_HPP */
