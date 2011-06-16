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

#ifndef EPIC_DAO_PERSON_DAO_HPP
#define EPIC_DAO_PERSON_DAO_HPP

#include "dao/MealFood.hpp"
#include "Statement.hpp"
#include "libcsv/MatchedValues.hpp"

namespace Epic
{
    namespace DAO 
    {   
        class Person;   //forward declaration
        class Weight;   //forward declaration
        class Food;     //forward declaration
        class Cereal;   //forward declaration
        class Milk;     //forward declaration
        class Portion;  //forward declaration
        class Meal;     //forward declaration
        class Frequency;//forward declaration
    } // DAO namespace

    namespace PersonDAO
    {
        class DataAccess
        {
        public:
            DataAccess() : 
                m_insert("INSERT INTO people(reference) VALUES (?) ;"),
                m_attach_weight("INSERT INTO person_weights(person_id,weight_id,modifier) VALUES (?,?,?) ;"),
                m_attach_cereal("INSERT INTO person_cereals(person_id,food_id,weight_id,modifier) VALUES (?,?,?,?) ;"),
                m_attach_fat("INSERT INTO person_fats(person_id,food_id,modifier) VALUES (?,?,?) ;"),
                m_attach_frequency("INSERT INTO person_meals(person_id,meal_id,frequency_id) VALUES (?,?,?) ;"),
                /* having to hard code the frequency id for milk is disgusting, move to config and have 
                 * milk load it on construction */
                m_attach_food("INSERT INTO person_foods(person_id,food_id,portion_id,frequency_id) VALUES (?,?,?,6) ;"),
                m_find_by_reference("SELECT id FROM people WHERE reference = ? ;"),
                m_find_by_id("SELECT reference FROM people WHERE id = ? ;") { }

            // find a person given an id
            bool find(sqlite3_int64 id, Epic::DAO::Person & person);

            // find a person given a reference
            bool find(const std::string & reference, Epic::DAO::Person & person);

            // save a person
            bool save(Epic::DAO::Person & person);
            
            // associate a person with a visible fat weighting
            bool attach_weight(const Epic::DAO::Person & person,const Epic::DAO::Weight & weight);

            // associate a person with a frying fat food code
            bool attach_frying_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food);

            // associate a person with a baking fat food code
            bool attach_baking_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food);
    
            // associate a person with a cereal food code
            bool attach_cereal(const Epic::DAO::Person & person,const Epic::DAO::Cereal & cereal);
 
            // associate a person with a milk food code and portion
            bool attach_milk(const Epic::DAO::Person & person,const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion);
 
            // associate a person with a meal frequency
            bool attach_meal(const Epic::DAO::Person & person,const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency);
        private:
            // associate a person with a fat
            bool attach_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food, const Epic::DAO::MealFood::Types type);

            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_attach_weight;
            Epic::Database::PreparedStatement m_attach_cereal;
            Epic::Database::PreparedStatement m_attach_fat;
            Epic::Database::PreparedStatement m_attach_frequency;
            Epic::Database::PreparedStatement m_attach_food;
            Epic::Database::PreparedStatement m_find_by_reference;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        bool find_by_reference(const std::string & reference, Epic::DAO::Person & person);
        bool find_by_id(sqlite3_int64, Epic::DAO::Person & person);
        bool save(Epic::DAO::Person & person);

        // associate a person with a visible fat weighting
        bool attach_weight(const Epic::DAO::Person & person,const Epic::DAO::Weight & weight);

        // associate a person with a frying fat food code
        bool attach_frying_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food);

        // associate a person with a baking fat food code
        bool attach_baking_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food);

        // associate a person with a cereal food code
        bool attach_cereal(const Epic::DAO::Person & person,const Epic::DAO::Cereal & cereal);

        // associate a person with a milk food code and portion
        bool attach_milk(const Epic::DAO::Person & person,const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion);

        // associate a person with a meal frequency
        bool attach_meal(const Epic::DAO::Person & person,const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency);

    } // PersonDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_PERSON_DAO_HPP */
