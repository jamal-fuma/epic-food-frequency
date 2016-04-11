#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "config/Global.hpp"

void test_creating_a_meal_called_apple();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();
    test_creating_a_meal_called_apple();
    return EXIT_SUCCESS;
}

void
test_creating_a_meal_called_apple()
{
    Epic::DAO::Meal meal ;
    meal.set_name("Apple");
    meal.set_description("apple treat");
    assert(meal.save() && "Saving should not fail");
}
