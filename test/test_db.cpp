#include "Epic_lib.hpp"

#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"


void test_person();
void test_questionaire();

int
main(int argc, char **argv)
{
    std::string conf    = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";
    std::string schema  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/sql/model.sql" ;
    std::string dbase   = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/test/foods.db" ;

    if(!Epic::Config::load(conf))
    {
        std::ostringstream ss;
        ss << "Config file missing " << conf << std::endl;
        Epic::Logging::error(ss.str());
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
    test_person();

    return EXIT_SUCCESS;
}

void test_person()
{
    Test::Person::Person person ;

    person.set_reference("foo");
    person.save();

    std::cout << "1st " << person;
 
    person.set_reference("bar");
    person.save();

    std::cout << "2nd " << person;

    Test::Person::Person foo = Test::Person::Person::find_by_reference("foo");
    
    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");
    
    std::cout << "Person [ok]" << std::endl;
}

void test_questionaire()
{
    Test::Questionaire::Questionaire questionaire ;

    questionaire.set_filename("foo");
    questionaire.save();

    std::cout << "1st " << questionaire;
 
    questionaire.set_filename("bar");
    questionaire.save();

    std::cout << "2nd " << questionaire;

    Test::Questionaire::Questionaire foo = Test::Questionaire::Questionaire::find_by_filename("foo");
    
    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");

    std::cout << "Questionaire [ok]" << std::endl;
}

