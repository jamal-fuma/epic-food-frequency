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

#ifndef EPIC_DAO_FOOD_DAO_HPP
#define EPIC_DAO_FOOD_DAO_HPP

#include "Statement.hpp"
#include "dao/FoodNutrient.hpp"
#include "dao/Food.hpp"
#include <vector>

namespace Epic
{
    namespace DAO 
    {   
        class Nutrient; //forward declaration
    } // DAO namespace

    namespace FoodDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO foods (name,description) VALUES (?,?) ;"),
                m_find_all("SELECT id,name,description from foods ;"),
                m_attach("INSERT INTO food_nutrients (food_id,nutrient_id,amount) VALUES (?,?,?) ;"),
                m_find_by_name("SELECT id,description FROM foods WHERE name = ? ;"),
                m_find_nutrients_by_food_id("SELECT food_nutrients.nutrient_id,food_nutrients.amount,nutrients.code "
                                            "FROM food_nutrients INNER JOIN nutrients ON food_nutrients.nutrient_id = nutrients.id "
                                            "WHERE food_nutrients.food_id = ? and food_nutrients.amount > 0;"),
                m_find_by_id("SELECT name,description FROM foods WHERE id = ? ;") { }


            // find a food given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Food & food);

            // find a food given a name
            bool find_by_name(const std::string & name, Epic::DAO::Food & food);
 
            // associate a food with a nutrient
            bool attach(const Epic::DAO::Food & food, const Epic::DAO::Nutrient & nutrient, double amount) ;

            // find all nutrients associated with a food
            bool find_nutrients(const Epic::DAO::Food & food, std::vector<Epic::DAO::FoodNutrient> & nutrients) ;
 
            // find all foods
            bool find_all(std::vector<Epic::DAO::Food> & foods);

            // save a food
            bool save(Epic::DAO::Food & food);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_attach;
            Epic::Database::PreparedStatement m_find_all;
            Epic::Database::PreparedStatement m_find_by_name;
            Epic::Database::PreparedStatement m_find_nutrients_by_food_id;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        // find a food given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Food & food);

        // find a food given a name
        bool find_by_name(const std::string & name, Epic::DAO::Food & food);

        // find all foods
        bool find_all(std::vector<Epic::DAO::Food> & foods);

        // save a food
        bool save(Epic::DAO::Food & food);

        // associate a food with a nutrient
        bool attach(const Epic::DAO::Food & food, const Epic::DAO::Nutrient & Nutrient, double amount) ;

        // find all nutrients associated with a food
        bool find_nutrients(const Epic::DAO::Food & food, std::vector<Epic::DAO::FoodNutrient> & nutrients) ;
    } // FoodDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_FOOD_DAO_HPP */
