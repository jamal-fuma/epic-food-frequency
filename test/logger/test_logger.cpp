#include "libhelper/Logger"

#include <stdlib.h>
int
main(int argc, char **argv)
{
    if(!Epic::Logging::open("log.out"))
        return EXIT_FAILURE;
    
    return EXIT_SUCCESS;
}

