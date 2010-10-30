#ifndef EPIC_CONFIG_READER_HPP
#define EPIC_CONFIG_READER_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <map>

namespace Epic
{
    namespace Config
    {
        class Reader
        {
                std::ifstream                       m_conf;
                std::map<std::string,std::string>   m_params;
         public:
                typedef std::map<std::string,std::string> value_type;
                typedef value_type::const_iterator const_iterator;

                Reader(const std::string & filename) : m_conf(filename.c_str()) { }

                bool load( value_type & dest);

                friend std::ostream & 
                operator << (std::ostream & out, const Reader & cnf)
                {
                    const_iterator end = cnf.m_params.end();
                    for(const_iterator cur = cnf.m_params.begin(); cur != end; ++cur)
                    {
                        out << cur->first  <<  "=\""  << cur->second  << "\"" << std::endl;
                    }
                    return out;
                }

                
                bool
                configured_value(const std::string & key,std::string & s);
        
                void
                insert(const std::string & key,const std::string & value)
                {
                    m_params[key] = value;
                }

            private:
                void
                strip_quotes(std::string & str);
                
                bool
                has_values(const std::string & line);
                
                void
                parse_line(const std::string & line, std::string & key, std::string & value);
        };
    } // Epic::Config
} // Epic
#endif /*ndef EPIC_CONFIG_READER_HPP */
