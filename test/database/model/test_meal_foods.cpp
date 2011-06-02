#include "config/Global.hpp"
#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "dao/MealFood.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "config/Global.hpp"

void test_creating_a_meal_food_should_fail();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();
    test_creating_a_meal_food_should_fail();
    return EXIT_SUCCESS;
}

void
test_creating_a_meal_food_should_fail()
{
    Epic::DAO::MealFood meal_food;

    meal_food.set_meal_id(Epic::DAO::Meal::find_by_name("foo").get_id());
    meal_food.set_modifier("a");
    meal_food.save();
    assert("meal_food should be invalid" && !meal_food.valid());
}
