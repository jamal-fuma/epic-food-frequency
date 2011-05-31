/* This file is part of the libEpic client */

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

#include "MealNutrientDAO.hpp"
#include "dao/MealNutrient.hpp"

// find agregated meal nutrient data
bool Epic::MealNutrientDAO::DataAccess::find_all_meal_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients)
{
    std::vector<Epic::DAO::MealNutrient> tmp;
    tmp.swap(meal_nutrients);
    for(int rc = m_find_meal_nutrients.step(); (SQLITE_ROW == rc); rc = m_find_meal_nutrients.step())
    {
        Epic::DAO::MealNutrient meal_nutrient;
        meal_nutrient.set_meal_id(m_find_meal_nutrients.column_int64(0));
        meal_nutrient.set_nutrient_code(m_find_meal_nutrients.column_int64(1));
        meal_nutrient.set_quantity(m_find_meal_nutrients.column_double(2));

        meal_nutrients.push_back(meal_nutrient);
    }
    m_find_meal_nutrients.reset();
    return (meal_nutrients.size() > 0 );
}

// find agregated nutrient data
bool Epic::MealNutrientDAO::DataAccess::find_all_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients)
{
    std::vector<Epic::DAO::MealNutrient> tmp;
    tmp.swap(meal_nutrients);
    for(int rc = m_find_nutrients.step(); (SQLITE_ROW == rc); rc = m_find_nutrients.step())
    {
        Epic::DAO::MealNutrient meal_nutrient;
        meal_nutrient.set_meal_id(0);
        meal_nutrient.set_nutrient_code(m_find_nutrients.column_int64(0));
        meal_nutrient.set_quantity(m_find_nutrients.column_double(1));
        meal_nutrients.push_back(meal_nutrient);
    }
    m_find_nutrients.reset();
    return (meal_nutrients.size() > 0 );
}

// find agregated nutrient data
bool Epic::MealNutrientDAO::DataAccess::save(const Epic::DAO::MealNutrient & meal_nutrient)
{
    m_insert.bind_int64(1,meal_nutrient.get_meal_id());
    m_insert.bind_int64(2,meal_nutrient.get_nutrient_code());
    m_insert.bind_double(3,meal_nutrient.get_quantity());
    int rc = m_insert.step();
    m_insert.reset();
    if(rc == SQLITE_DONE)
        return true;

    return false;
}

// clear agregated nutrient data
void Epic::MealNutrientDAO::DataAccess::clear()
{
    m_clear.step();
    m_clear.reset();
}

void Epic::MealNutrientDAO::clear()
{
   Epic::Pattern::Singleton< Epic::MealNutrientDAO::DataAccess >::instance().clear();
}

bool Epic::MealNutrientDAO::find_all_meal_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients)
{
   return Epic::Pattern::Singleton< Epic::MealNutrientDAO::DataAccess >::instance().find_all_meal_nutrients(meal_nutrients);
}

bool Epic::MealNutrientDAO::find_all_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients)
{
   return Epic::Pattern::Singleton< Epic::MealNutrientDAO::DataAccess >::instance().find_all_nutrients(meal_nutrients);
}

bool Epic::MealNutrientDAO::save(const Epic::DAO::MealNutrient & meal_nutrient)
{
   return Epic::Pattern::Singleton< Epic::MealNutrientDAO::DataAccess >::instance().save(meal_nutrient);
}


bool Epic::DAO::MealNutrient::find_all_meal_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients)
{
   return Epic::MealNutrientDAO::find_all_meal_nutrients(meal_nutrients);
}

bool Epic::DAO::MealNutrient::find_all_nutrients(std::vector<Epic::DAO::MealNutrient> & meal_nutrients)
{
   return Epic::MealNutrientDAO::find_all_nutrients(meal_nutrients);
}

bool Epic::DAO::MealNutrient::save()
{
   return Epic::MealNutrientDAO::save(*this);
}
void Epic::DAO::MealNutrient::clear()
{
    Epic::MealNutrientDAO::clear();
}

