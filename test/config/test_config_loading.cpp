#include "config/Global.hpp"

#include <stdlib.h>

int
main(int argc, char **argv)
{

    std::string conf    =  DDEFAULT_CONFIG_FILE;

    if(!Epic::Config::load(conf))
    {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


 
