#ifndef EPIC_DAO_MEAL_NUTRIENT_HPP
#define EPIC_DAO_MEAL_NUTRIENT_HPP

#include "Epic_lib.hpp"
#include <vector>

namespace Epic
{
    namespace DAO
    {
        class MealNutrient
        {
            public:
                MealNutrient() : 
                    m_meal_id(0), m_nutrient_code(0), m_amount(0), m_quantity(0) {}

                // accessors
                sqlite3_int64 get_meal_id() const {
                    return m_meal_id;
                }

                sqlite3_int64 get_nutrient_code() const {
                    return m_nutrient_code;
                }

                double get_amount() const {
                    return m_amount;
                }

                double get_quantity() const {
                    return m_quantity;
                }

                // mutators
                void set_meal_id(sqlite3_int64 meal_id) {
                    m_meal_id = meal_id;
                }

                void set_nutrient_code(sqlite3_int64 nutrient_code) {
                    m_nutrient_code = nutrient_code;
                }

                void set_amount(double amount) {
                    m_amount = amount;
                }

                void set_quantity(double quantity) {
                    m_quantity = quantity;
                }

                bool save();
                static void clear();
                static bool find_all_meal_nutrients(std::vector<MealNutrient> & meal_nutrients);
                static bool find_all_nutrients(std::vector<MealNutrient> & meal_nutrients);

            private:
                sqlite3_int64 m_meal_id;
                sqlite3_int64 m_nutrient_code;
                double        m_amount;
                double        m_quantity;
        };

    } // DAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_MEAL_NUTRIENT_HPP */
