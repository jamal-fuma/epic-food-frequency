#include "Epic_lib.hpp"

#include "Questionaire.hpp"

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

    return EXIT_SUCCESS;
}

