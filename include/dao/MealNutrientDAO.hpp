#ifndef EPIC_DAO_MEAL_NUTRIENT_DAO_HPP
#define EPIC_DAO_MEAL_NUTRIENT_DAO_HPP

#include "Epic_lib.hpp"
#include "dao/MealNutrient.hpp"
#include <vector>

namespace Epic
{
    namespace MealNutrientDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO meal_nutrients_tmp (meal_id,nutrient_code,amount,quantity) VALUES (?,?,?,?) ;"),
                m_find_meal_nutrients("SELECT meal_id,amount,nutrient_code,quantity FROM meal_nutrients_vw ;"),
                m_find_nutrients("SELECT nutrient_code,quantity FROM nutrients_vw ;"),
                m_clear("DELETE FROM meal_nutrients_tmp ;") {}

            bool find_all_meal_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients);
            bool find_all_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients);
            bool save(const Epic::DAO::MealNutrient & meal_nutrient);
            void clear();

        private:
            Epic::Database::PreparedStatement m_clear; 
            Epic::Database::PreparedStatement m_insert; 
            Epic::Database::PreparedStatement m_find_meal_nutrients; 
            Epic::Database::PreparedStatement m_find_nutrients; 
        };

        bool find_all_meal_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients);
        bool find_all_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients);
        bool save(const Epic::DAO::MealNutrient & meal_nutrient);
        void clear();
    
    } // MealNutrientDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_MEAL_NUTRIENT_DAO_HPP */
