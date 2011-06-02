#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "dao/Nutrient.hpp"
#include "config/Global.hpp"

void test_creating_a_nutrient_with_code_65();
void test_finding_a_previously_created_nutrient_with_code_65();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();

    test_creating_a_nutrient_with_code_65();
    test_finding_a_previously_created_nutrient_with_code_65();

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
