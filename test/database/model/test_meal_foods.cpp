#include <iterator>
#include <fstream>

#include "Epic_lib.hpp"

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
    std::string conf    = "./client.conf";
    std::string schema  = "./sql/model.sql" ;
    std::string dbase   = "./foods.db" ;
    
    if(!Epic::Config::load(conf))
    {
        Epic::Logging::Error().log() << "Config file missing " << conf << std::endl;
        return EXIT_FAILURE;
    }

    // overwrite the schema variable listed in config file with the new version
    if(!Epic::Config::insert("schema",schema,true))
        return EXIT_FAILURE;

    // overwrite the database variable listed in config file with the new version
    if(!Epic::Config::insert("database",dbase,true))
        return EXIT_FAILURE;

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

    // load file into string
    std::ifstream infile(schema.c_str());
    std::string data(
            (std::istreambuf_iterator<char>(infile)),
            std::istreambuf_iterator<char>()
            );
    
    std::cout << data << std::endl;
    
    return EXIT_SUCCESS;
}

