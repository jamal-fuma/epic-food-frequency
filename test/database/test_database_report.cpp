
#include <stdlib.h>
#include <iomanip>
#include <assert.h>
#undef NDEBUG

#include "config/Global.hpp"
#include "libhelper/Logger.hpp"
#include "libdao/Database.hpp"

#include "dao/Nutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/MealFood.hpp"

std::string double_to_formatted_string(double value);

std::string double_to_formatted_string(double value)
{
    std::stringstream str;
    str << std::fixed << std::setprecision(7) << value;
    return str.str();
}

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;
    assert(Epic::Config::load(conf) && "Config loading should not fail");
    assert(Epic::Logging::open("log.out") && "Opening Log should not fail");

    // connect to database which should seed the tables
    assert(Epic::Database::connect()
            && "Opening the DB should work, right?");

    // find all meals
    std::vector<Epic::DAO::Meal> meals;
    if(Epic::DAO::Meal::find_all(meals))
    {
        std::vector<Epic::DAO::Meal>::const_iterator ci,end;
        end = meals.end();
        std::vector<Epic::DAO::MealFood> foods;
        std::vector<Epic::DAO::FoodNutrient> nutrients;

        // find foods for meals
        for(ci  = meals.begin(); ci != end; ++ci)
        {
            foods.clear();
            std::cout << *ci;
            if(ci->find_all_foods(foods))
            {
                // find nutrients for foods
                std::vector<Epic::DAO::MealFood>::const_iterator food_it, food_end;
                food_end = foods.end();
                for(food_it  = foods.begin(); food_it != food_end; ++food_it)
                {
                    Epic::DAO::Food food = Epic::DAO::Food::find_by_id(food_it->get_food_id());
                    std::cout << "\n\t Food: " <<  food.get_name() << " " << double_to_formatted_string(food_it->get_amount()) << "(g)\n" ;
                    if(food.get_id() > 0)
                    {
                        if(food.find_nutrients(nutrients))
                        {
                            std::vector<Epic::DAO::FoodNutrient>::const_iterator nutrient_it, nutrient_end;
                            nutrient_end = nutrients.end();
                            for(nutrient_it  = nutrients.begin(); nutrient_it != nutrient_end; ++nutrient_it)
                            {
                                Epic::DAO::Nutrient tmp_nutrient = Epic::DAO::Nutrient::find_by_id(nutrient_it->get_nutrient_id()) ;
                                std::cout << "\t \t \t Nutrient: (" << tmp_nutrient.get_code()  << ") " << tmp_nutrient.get_description() << " " << double_to_formatted_string(nutrient_it->get_amount()) << " (" << tmp_nutrient.get_units() << ")\n";
                            }
                            std::cout << std::endl;
                        }
                    }
                }
            }
        }
    }

    return EXIT_SUCCESS;
}
