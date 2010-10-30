#ifndef EPIC_CLIENT_VERSION_HPP
#define EPIC_CLIENT_VERSION_HPP

#include "../include/license/License.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace Epic
{
    namespace Client
    {
        struct Version
        {
            int m_major;
            int m_minor;
            std::string m_license;
            Version() : m_major(0), m_minor(1), m_license(GPL_LICENSE_TEXT) {} 

            int
            operator()()
            {
                fprintf(stderr,"Version %d.%d\nLicence:\n%s\n",
                        m_major,
                        m_minor,
                        m_license.c_str());
                return EXIT_SUCCESS;
            }
        };

    } // Epic::Client
} // Epic

#endif /* EPIC_CLIENT_VERSION_HPP */

