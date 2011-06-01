#include "config/Global.hpp"
#include <stdlib.h>
#include <assert.h>

#include "util.h"

#include "libhelper/Logger.hpp"

#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"
#include "dao/Nutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/Weight.hpp"
#include "dao/Frequency.hpp"
#include "dao/MealFood.hpp"
#include "dao/Portion.hpp"
#include "dao/Cereal.hpp"


void test_person();
void test_weights();
void test_nutrient();
void test_questionaire();
void test_meal();
void test_food();
void test_frequencies();
void test_questionaire_person();
void test_food_nutrient();
void test_meal_foods();
void test_portions();
void test_cereals();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    if(!Epic::Config::load(conf))
    {
        return EXIT_FAILURE;
    }
    
    Epic::Database::connect();

    test_questionaire();
    test_nutrient();
    test_meal();
    test_food();
    test_weights();
    test_person();
    test_frequencies();
    test_questionaire_person();
    test_food_nutrient();
    test_meal_foods();
    test_portions();

    return EXIT_SUCCESS;
}

