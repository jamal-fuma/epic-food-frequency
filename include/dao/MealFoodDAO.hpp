#ifndef EPIC_DAO_MEAL_FOOD_DAO_HPP
#define EPIC_DAO_MEAL_FOOD_DAO_HPP

#include "Epic_lib.hpp"
#include "dao/MealFood.hpp"
#include <vector>

namespace Epic
{
    namespace MealFoodDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO meal_foods (meal_id,food_id,modifier,amount) VALUES (?,?,?,?) ;"),
                m_find_all_by_meal_id("SELECT id,food_id,modifier,amount FROM meal_foods WHERE meal_id = ? ;"),
                m_find_all_by_modifier("SELECT id,meal_id,food_id,amount FROM meal_foods WHERE modifier = ? ;"),
                m_find_all_by_meal_id_and_modifier("SELECT id,food_id,amount FROM meal_foods WHERE meal_id = ? and modifier = ? ;"),
                m_find_by_id("SELECT meal_id,food_id,modifier,amount from meal_foods WHERE id = ? ;") { }

                // find all meal_foods with specified meal_id
                bool find_all_by_meal_id(sqlite3_int64 meal_id, std::vector<Epic::DAO::MealFood> & meal_foods);
               
                // find all meal_foods with specified modifier
                bool find_all_by_modifier(int modifier, std::vector<Epic::DAO::MealFood> & meal_foods);
                 
                // find all meal_foods with specified modifier and meal
                bool find_all_by_meal_id_and_modifier(sqlite3_int64 meal_id, int modifier, std::vector<Epic::DAO::MealFood> & meal_foods);
 
                // find meal_food with specified id
                bool find_by_id(sqlite3_int64 id, Epic::DAO::MealFood & meal_food);
                   
                // save meal_food 
                bool save(Epic::DAO::MealFood & meal_food);
                
        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_all_by_meal_id;
            Epic::Database::PreparedStatement m_find_all_by_meal_id_and_modifier;
            Epic::Database::PreparedStatement m_find_all_by_modifier;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        // find meal_food with specified id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::MealFood & meal_food);

        // save meal_food 
        bool save(Epic::DAO::MealFood & meal_food);
        
        // find all meal_foods with specified meal_id
        bool find_all_by_meal_id(sqlite3_int64 meal_id, std::vector<Epic::DAO::MealFood> & meal_foods);

        // find all meal_foods with specified modifier
        bool find_all_by_modifier(int modifier, std::vector<Epic::DAO::MealFood> & meal_foods);

        // find all meal_foods with specified modifier and meal
        bool find_all_by_meal_id_and_modifier(sqlite3_int64 meal_id, int modifier, std::vector<Epic::DAO::MealFood> & meal_foods);

    } // MealFoodDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_MEAL_FOOD_DAO_HPP */
