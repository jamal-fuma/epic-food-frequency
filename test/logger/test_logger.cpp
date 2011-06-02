#include <stdlib.h>
#include <assert.h>
#undef NDEBUG

#include "libhelper/Logger.hpp"

int
main(int argc, char **argv)
{
    assert(Epic::Logging::open("log.out") && "Opening Log should not fail");
    
    return EXIT_SUCCESS;
}

