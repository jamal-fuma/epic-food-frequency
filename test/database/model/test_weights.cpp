#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "libdao/util.h"
#include "config/Global.hpp"
#include "dao/Weight.hpp"

void test_creating_weights();
void test_weights_have_correct_ids_and_values();
void test_weights_have_correct_upper_and_lower_bounds();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();

    test_weights_have_correct_ids_and_values();
    test_weights_have_correct_upper_and_lower_bounds();
    test_creating_weights();
    return EXIT_SUCCESS;
}

void test_creating_weights()
{
    Epic::DAO::Weight weight ;
    double d_list[] = { 1.00, 0.50, 0.0, 0.0, 0.0 };

    double *end_p = ( &d_list[0] + ( sizeof(d_list) / sizeof(*d_list) ) );
    for(double *p = &d_list[0]; p != end_p; ++p)
    {
        weight.set_amount(*p);
        assert(weight.save() && "Creating Weights should not fail");
    }
}

void test_weights_have_correct_ids_and_values()
{
    Epic::DAO::Weight weight  = Epic::DAO::Weight::find_by_id(1);
    assert("weight should be valid and have amount equal to 1.0" && weight.valid() && utility_same_double(weight.get_amount(),1.0));

    weight  = Epic::DAO::Weight::find_by_id(2);
    assert("weight should be valid and have amount equal to 0.5" && weight.valid() && utility_same_double(weight.get_amount(),0.5));

    for(int x = 3; x<6; ++x)
    {
        weight  = Epic::DAO::Weight::find_by_id(x);
        assert("weight should be valid and have amount equal to 0.0" && weight.valid() && utility_same_double(weight.get_amount(),0.0));
    }
}

void test_weights_have_correct_upper_and_lower_bounds()
{
    sqlite3_int64 upper,lower;
    Epic::DAO::Weight::find_bounds(upper,lower);
    assert("weight upper bounds should be == 5 " && upper == 5);
    assert("weight lower bounds should be == 1 " && lower == 1);
}
