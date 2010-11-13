#ifndef EPIC_CONFIG_QUANTITY_HPP
#define EPIC_CONFIG_QUANTITY_HPP

#include "../include/pattern/Singleton.hpp"
#include "../include/config/Reader.hpp"
#include <stdexcept>
#include <algorithm> // std::copy
namespace Epic
{
    namespace Config
    {
        // purely to avoid conficts with application config
        class Code
        {
        public:
            Code() {}
            Code(const std::string & code) : m_value(code) {}
            operator std::string() const
            {
                return m_value;
            }

            template <class T>
            Code &
            operator=(const T & rhs)
            {
                m_value = rhs;
                return *this;
            }
            private:
                std::string m_value;
        };

        class Quantity
        {
         public:
                typedef std::map<std::string,Code> value_type;
                typedef value_type::const_iterator const_iterator;

                Quantity() :
                    m_params(Epic::Pattern::Singleton< value_type >::instance())
                {
                }

                // should only be called once
                void load(const std::string & filename)
                {
                    Epic::Config::Reader rdr(filename);
                    Epic::Config::Reader::value_type params;
                    if(!rdr.load(params))
                        throw std::runtime_error("failed to load configuration from " + filename);

                    // copy over to final storage
                    Epic::Config::Reader::const_iterator cur, end;
                    cur = params.begin();
                    end = params.end();
                    for(; cur != end; ++cur)
                    {
                        m_params[cur->first] = cur->second;
                    }
                }

                static
                value_type & config()
                {
                    return Epic::Pattern::Singleton< value_type >::instance();
                }
        private:
                value_type & m_params;
        };
    } // Epic::Config
} // Epic
#endif /*ndef EPIC_CONFIG_QUANTITY_HPP */

