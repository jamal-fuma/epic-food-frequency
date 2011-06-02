#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "config/Global.hpp"

#include "dao/Food.hpp"

void test_creating_a_food_called_foo();
void test_finding_a_previously_created_food_called_foo();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();

    test_creating_a_food_called_foo();
    test_finding_a_previously_created_food_called_foo();

    return EXIT_SUCCESS;
}

void
test_creating_a_food_called_foo()
{
    Epic::DAO::Food food ;
    food.set_name("foo");
    food.set_description("foo desc");
    assert(food.save() && "Saving should not fail");
}

void
test_finding_a_previously_created_food_called_foo()
{
    Epic::DAO::Food foo = Epic::DAO::Food::find_by_name("foo");
    assert(foo.valid() && "Foo is expected to be valid");
}
