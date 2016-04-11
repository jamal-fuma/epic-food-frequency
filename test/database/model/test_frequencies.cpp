#include <stdlib.h>
#include <assert.h>

#include "libdao/util.h"
#include "libdao/Database.hpp"
#include "config/Global.hpp"
#include "dao/Frequency.hpp"

void test_creating_frequencies();
void test_frequencies_have_correct_ids_and_values();
void test_frequencies_have_correct_upper_and_lower_bounds();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();

    test_frequencies_have_correct_ids_and_values();
    test_frequencies_have_correct_upper_and_lower_bounds();

    test_creating_frequencies();
    return EXIT_SUCCESS;
}

void test_creating_frequencies()
{
    Epic::DAO::Frequency frequency ;
    double d_list[] = { 0.0, 0.07, 0.14, 0.43, 0.79, 1.00, 2.50, 4.50, 6.0 };

    double *end_p = ( &d_list[0] + ( sizeof(d_list) / sizeof(*d_list) ) );
    for(double *p = &d_list[0]; p != end_p; ++p)
    {
        frequency.set_amount(*p);
        assert(frequency.save() && "Creating Frequencies should not fail");
    }
}

void test_frequencies_have_correct_ids_and_values()
{
    Epic::DAO::Frequency frequency  = Epic::DAO::Frequency::find_by_id(1);
    assert("frequency should be valid and have amount equal to 0.0" && frequency.valid() && utility_same_double(frequency.get_amount(),0.0));

    frequency  = Epic::DAO::Frequency::find_by_id(2);
    assert("frequency should be valid and have amount equal to 0.07" && frequency.valid() && utility_same_double(frequency.get_amount(),0.07));

    frequency  = Epic::DAO::Frequency::find_by_id(3);
    assert("frequency should be valid and have amount equal to 0.14" && frequency.valid() && utility_same_double(frequency.get_amount(),0.14));

    frequency  = Epic::DAO::Frequency::find_by_id(4);
    assert("frequency should be valid and have amount equal to 0.43" && frequency.valid() && utility_same_double(frequency.get_amount(),0.43));

    frequency  = Epic::DAO::Frequency::find_by_id(5);
    assert("frequency should be valid and have amount equal to 0.79" && frequency.valid() && utility_same_double(frequency.get_amount(),0.79));

    frequency  = Epic::DAO::Frequency::find_by_id(6);
    assert("frequency should be valid and have amount equal to 1.00" && frequency.valid() && utility_same_double(frequency.get_amount(),1.00));

    frequency  = Epic::DAO::Frequency::find_by_id(7);
    assert("frequency should be valid and have amount equal to 2.50" && frequency.valid() && utility_same_double(frequency.get_amount(),2.50));

    frequency  = Epic::DAO::Frequency::find_by_id(8);
    assert("frequency should be valid and have amount equal to 4.50" && frequency.valid() && utility_same_double(frequency.get_amount(),4.50));

    frequency  = Epic::DAO::Frequency::find_by_id(9);
    assert("frequency should be valid and have amount equal to 6.00" && frequency.valid() && utility_same_double(frequency.get_amount(),6.00));
}

void test_frequencies_have_correct_upper_and_lower_bounds()
{
    sqlite3_int64 upper,lower;
    Epic::DAO::Frequency::find_bounds(upper,lower);
    assert("frequency upper bounds should be == 9 " && upper == 9);
    assert("frequency lower bounds should be == 1 " && lower == 1);
}
