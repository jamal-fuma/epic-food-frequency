#include "config/Global.hpp"
#include "libdao/Database.hpp"

#include <stdlib.h>
#include <assert.h>
#include <string>
#undef NDEBUG

int
main(int argc, char **argv)
{
    // load same config we ship to users
    std::string conf    =  DEFAULT_CONFIG_FILE;
    assert(Epic::Config::load(conf) 
            && "Config loading should not fail");
    
    // connect to database which should seed the tables
    assert(Epic::Database::connect();
            && "Opening the DB should work, right?");

    return EXIT_SUCCESS;
}
