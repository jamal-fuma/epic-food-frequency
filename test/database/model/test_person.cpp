#include "config/Global.hpp"
#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "config/Global.hpp"
#include "dao/Person.hpp"
#include "dao/Weight.hpp"

void test_creating_a_person_with_reference_foo();
void test_creating_a_weight();
void test_finding_a_previously_created_weight_with_id_of_1();
void test_finding_a_previously_created_person_with_reference_foo();
void test_attaching_visible_fat_to_person();
    
int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();

    test_creating_a_person_with_reference_foo();
    test_creating_a_weight();
    test_finding_a_previously_created_weight_with_id_of_1();
    test_finding_a_previously_created_person_with_reference_foo();
    test_attaching_visible_fat_to_person();
    return EXIT_SUCCESS;
}

void
test_creating_a_person_with_reference_foo()
{
    Epic::DAO::Person person ;
    person.set_reference("foo");
    assert(person.save() && "Saving a Person should not fail");
}

void
test_creating_a_weight()
{
    Epic::DAO::Weight weight; 
    weight.set_amount(55.0);
    assert(weight.save() && "Saving a Weight should not fail");
    assert( (weight.get_id() == 1) && "Weight should have id of 1");
}

void
test_finding_a_previously_created_weight_with_id_of_1()
{
    Epic::DAO::Weight weight = Epic::DAO::Weight::find_by_id(1);
    assert(weight.valid() && "previously created Weight with id 1 is expected to be valid");
}

void
test_finding_a_previously_created_person_with_reference_foo()
{
    Epic::DAO::Person person  = Epic::DAO::Person::find_by_reference("foo");
    assert(person.valid() && "previously created Person with reference foo is expected to be valid");
}

void
test_attaching_visible_fat_to_person()
{
    Epic::DAO::Weight weight = Epic::DAO::Weight::find_by_id(1);
    assert(weight.valid() && "previously created Weight with id 1 is expected to be valid");
    
    Epic::DAO::Person person  = Epic::DAO::Person::find_by_reference("foo");
    assert(person.valid() && "previously created Person with reference foo is expected to be valid");

    assert( person.attach_visible_fat(weight) && "Attaching a Visible fat weighting should not fail");
}

