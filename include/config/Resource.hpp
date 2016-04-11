#ifndef EPIC_CONFIG_RESOURCE_HPP
#define EPIC_CONFIG_RESOURCE_HPP

#include <iterator>
#include <fstream>
#include <string>
#include <stdexcept>

namespace Epic
{
    namespace Config
    {
        namespace Resource
        {
            static void
            load( const std::string & filename, std::string & dest)
            {
                try
                {
                    std::ifstream infile(filename.c_str());

                    std::string data(
                            (std::istreambuf_iterator<char>(infile)),
                            std::istreambuf_iterator<char>()
                            );

                    dest.assign(data);
                }
                catch(...)
                {
                    throw std::runtime_error("Loading Resource from file failed");
                }
            }
        }
    } // Epic::Config
} // Epic
#endif /*ndef EPIC_CONFIG_RESOURCE_HPP */
