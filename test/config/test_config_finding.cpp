#include "config/Global.hpp"

#include <stdlib.h>
#include <assert.h>
#undef NDEBUG

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    // overwrite the schema variable listed in config file with the new version
    assert(Epic::Config::insert("schema","DUMMY",true) && 
            "Values insertion should not fail");
    // grab the contents
    std::string s;
    assert(Epic::Config::find("schema",s) && "Values that have been just inserted should be found");

    return (s == "DUMMY") ? EXIT_SUCCESS : EXIT_FAILURE;
}


