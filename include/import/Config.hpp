#ifndef EPIC_IMPORT_CONFIG_HPP
#define EPIC_IMPORT_CONFIG_HPP

#include "import/Import.hpp"
#include "pattern/Singleton.hpp"

namespace Epic
{
    namespace Import
    {
 
        struct Config
        {
            static const size_t fields = 2;

            std::string m_name;
            std::string m_value;

            Config() {}
            Config( const std::string & name,
                    const std::string & value) : 
                m_name(name), 
                m_value(value) {}

            friend std::ostream & operator<<(std::ostream & rhs, const Config & conf)
            {
                rhs << conf.m_name << "=" << conf.m_value << std::endl;
                return rhs;
            }
            operator std::string() const
            {
                return m_value;
            }

        };

        struct ConfigData
        {
            typedef std::map<std::string,Config> value_map_t;
            value_map_t                         & m_configs_by_name;
         
            ConfigData() : m_configs_by_name( Epic::Pattern::Singleton<value_map_t>::instance())
            {
                
            }

            operator const value_map_t & () const
            {
                return m_configs_by_name;
            }

            bool
            operator()(std::string & error_message)
            {
                std::cerr << error_message << std::endl;
                return false;
            }

            // header
            bool operator()(str_vector_t & v)
            {
                // sufficent fields for format
                if(v.size() < Config::fields)
                {
                    std::cerr << "Unexpected number of fields in config file";
                    std::cerr << " expected at least " << Config::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }


                if(v[0] != "NAME")
                {
                    std::cerr << "Unexpected field in config file";
                    std::cerr << " expected first field to be 'NAME' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "VALUE")
                {
                    std::cerr << "Unexpected field in config file";
                    std::cerr << " expected second field to be 'VALUE' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }
                return true;
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                    if(v.size() == Config::fields)
                    {
                        Config c( const_cast<const std::string &>(v[0]),
                                const_cast<const std::string &>(v[1]));

                        m_configs_by_name[c.m_name] = c;
                    }
                    else
                    {
                        std::cerr << "Error in config file: ";
                        std::cerr << "skipping line :" << line;
                    }
                    return true;
            }

            friend std::ostream & operator<<(std::ostream & out, ConfigData & rhs)
            {
                value_map_t::const_iterator  it;
                value_map_t::const_iterator end;
                const value_map_t & map = rhs;
                it = map.begin(); end = map.end();
                for(; it != end; ++it)
                {
                    out << it->second;
                }
                return out;
            }
        };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_CONFIG_HPP */
