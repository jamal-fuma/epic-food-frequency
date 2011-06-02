#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "config/Global.hpp"

#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"

void test_creating_a_questionaire_called_foo();
void test_finding_a_previously_created_questionaire_called_foo();
void test_creating_a_person_with_reference_bar();
void test_attaching_person_to_questionaire();
void test_retrieving_person_called_bar_from_questionaire_with_ref_foo();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();

    test_creating_a_questionaire_called_foo();
    test_finding_a_previously_created_questionaire_called_foo();
    test_creating_a_person_with_reference_bar();
    test_attaching_person_to_questionaire();
    test_retrieving_person_called_bar_from_questionaire_with_ref_foo();
    
    return EXIT_SUCCESS;
}

void
test_creating_a_questionaire_called_foo()
{
    Epic::DAO::Questionaire questionaire ;
    questionaire.set_filename("foo");
    assert(questionaire.save() && "Saving Questionaire should not fail");
}

void
test_finding_a_previously_created_questionaire_called_foo()
{
    Epic::DAO::Questionaire questionaire = Epic::DAO::Questionaire.find_by_filename("foo");
    assert(questionaire.valid() && "Finding previously created Questionaire called foo should be valid");
}

void
test_creating_a_person_with_reference_bar()
{
    Epic::DAO::Person person ;
    person.set_reference("bar");
    assert(person.save() && "Saving a Person should not fail");
}

void
test_attaching_person_to_questionaire()
{
    Epic::DAO::Person person  = Epic::DAO::Person::find_by_reference("bar");
    assert(person.valid() && "previously created Person with reference foo is expected to be valid");

    Epic::DAO::Questionaire questionaire = Epic::DAO::Questionaire.find_by_filename("foo");
    assert(questionaire.valid() && "Finding previously created Questionaire called foo should be valid");
    
    assert( questionaire.attach(person) &&  "Attaching Person to the Questionaire should not fail");
}

void
test_retrieving_person_called_bar_from_questionaire_with_ref_foo()
{
    Epic::DAO::Person person  = Epic::DAO::Person::find_by_reference("bar");
    assert(person.valid() && "previously created Person with reference foo is expected to be valid");

    Epic::DAO::Questionaire questionaire = Epic::DAO::Questionaire.find_by_filename("foo");
    assert(questionaire.valid() && "Finding previously created Questionaire called foo should be valid");

    std::vector<Epic::DAO::Person> people;
    questionaire.find_people(people);
    assert(questionaire.find_people(people) && "Finding people on the questionaire is expected to work");

    assert(!people.empty() && "Expected [1] elements but instead was empty");
    assert( (people.size() == 1) && "Expected exactly [1] elements");
    assert( people.begin()->get_reference() == "bar") && "Expected person bar to be first entry");
    assert( people.begin()->get_reference() == "bar") && "Expected person bar to be last entry");
}
