#include "config/Global.hpp"

#include <stdlib.h>
#include <assert.h>
#undef NDEBUG


int
main(int argc, char **argv)
{

    std::string conf    =  DEFAULT_CONFIG_FILE;
    assert(Epic::Config::load(conf) && "Config loading should not fail");

    return EXIT_SUCCESS;
}


 
