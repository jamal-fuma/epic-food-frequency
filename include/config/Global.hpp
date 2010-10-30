#ifndef EPIC_CONFIG_GLOBAL_HPP
#define EPIC_CONFIG_GLOBAL_HPP

#include "../include/pattern/Singleton.hpp"
#include "../include/config/Reader.hpp"
#include <stdexcept>

namespace Epic
{
    namespace Config
    {
        class Global
        {
                std::map<std::string,std::string> & m_params;
         public:
                typedef std::map<std::string,std::string> value_type;
                typedef value_type::const_iterator const_iterator;

                Global() :
                    m_params(Epic::Pattern::Singleton< std::map<std::string,std::string> >::instance())
                {
                }

                // should only be called once
                void load(const std::string & filename)
                {
                    Epic::Config::Reader rdr(filename);
                    if(!rdr.load(m_params))
                        throw std::runtime_error("failed to load configuration from " + filename);
                }

                static
                value_type & config()
                {
                    return Epic::Pattern::Singleton< std::map<std::string,std::string> >::instance();
                }
        };
    } // Epic::Config
} // Epic
#endif /*ndef EPIC_CONFIG_GLOBAL_HPP */

