/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
                str_vector_t expected;
                expected.push_back("NAME");
                expected.push_back("VALUE");
                return Epic::Import::DBModel::same_header("config",expected,v);
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
