#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "dao/Food.hpp"
#include "dao/Nutrient.hpp"
#include "config/Global.hpp"

void test_creating_a_food_called_apple();

void test_creating_a_nutrient_with_code_65();
void test_finding_a_previously_created_nutrient_with_code_65();
void test_attaching_nutrient_65_to_food_called_apple();

void test_creating_a_nutrient_with_code_77();
void test_finding_a_previously_created_nutrient_with_code_77();
void test_attaching_nutrient_77_to_food_called_apple();

void test_retrieving_nutrients_65_and_77_from_food_called_apple();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();


    test_creating_a_nutrient_with_code_65();
    test_finding_a_previously_created_nutrient_with_code_65();

    test_creating_a_nutrient_with_code_77();
    test_finding_a_previously_created_nutrient_with_code_77();

    test_creating_a_food_called_apple();
    test_attaching_nutrient_65_to_food_called_apple();
    test_attaching_nutrient_77_to_food_called_apple();
    test_retrieving_nutrients_65_and_77_from_food_called_apple();

    return EXIT_SUCCESS;
}

void
test_creating_a_nutrient_with_code_65()
{
    Epic::DAO::Nutrient nutrient;
    nutrient.set_code(65);
    nutrient.set_description("foo bar");
    nutrient.set_units("grams");
    nutrient.save();
    assert(nutrient.save() && "Saving should not fail");
}

void
test_finding_a_previously_created_nutrient_with_code_65()
{
    Epic::DAO::Nutrient nutrient = Epic::DAO::Nutrient::find_by_code(65);
    assert(nutrient.valid() && "previously created Nutrient with code 65 is expected to be valid");
}

void
test_creating_a_nutrient_with_code_77()
{
    Epic::DAO::Nutrient nutrient;
    nutrient.set_code(77);
    nutrient.set_description("foo bar");
    nutrient.set_units("grams");
    nutrient.save();
    assert(nutrient.save() && "Saving should not fail");
}

void
test_finding_a_previously_created_nutrient_with_code_77()
{
    Epic::DAO::Nutrient nutrient = Epic::DAO::Nutrient::find_by_code(77);
    assert(nutrient.valid() && "previously created Nutrient with code 77 is expected to be valid");
}

void
test_creating_a_food_called_apple()
{
    Epic::DAO::Food food ;
    food.set_name("Apple");
    food.set_description("apple treat");
    assert(food.save() && "Saving should not fail");
}

void
test_attaching_nutrient_65_to_food_called_apple()
{
    Epic::DAO::Food food = Epic::DAO::Food::find_by_name("Apple");
    assert(food.valid() && "Food called apple is expected to be valid");

    Epic::DAO::Nutrient nutrient = Epic::DAO::Nutrient::find_by_code(65);
    assert(nutrient.valid() && "previously created Nutrient with code 65 is expected to be valid");

    assert( food.attach(nutrient,10.5) && "Attaching a Nutrient (code 65) to a Food (named apple) should not fail");
}

void
test_attaching_nutrient_77_to_food_called_apple()
{
    Epic::DAO::Food food = Epic::DAO::Food::find_by_name("Apple");
    assert(food.valid() && "Food called apple is expected to be valid");

    Epic::DAO::Nutrient nutrient = Epic::DAO::Nutrient::find_by_code(77);
    assert(nutrient.valid() && "previously created Nutrient with code 77 is expected to be valid");

    assert( food.attach(nutrient,55.5) && "Attaching a Nutrient (code 77) to a Food (named apple) should not fail");
}

void
test_retrieving_nutrients_65_and_77_from_food_called_apple()
{
    Epic::DAO::Food food = Epic::DAO::Food::find_by_name("Apple");
    assert(food.valid() && "Food called apple is expected to be valid");

    std::vector<Epic::DAO::FoodNutrient> nutrients;
    assert(food.find_nutrients(nutrients) && "Finding previously created Nutrients with codes [65,77] is expected to work");
    assert(!nutrients.empty() && "Expected [2] elements but instead was empty");
    assert( (nutrients.size() == 2) && "Expected exactly [2] elements");
    assert( nutrients.begin()->get_nutrient_code() == 65 && "Expected nutrient 65 to be first entry");
    assert( nutrients.rbegin()->get_nutrient_code() == 77 && "Expected nutrient 77 to be last entry");
}
