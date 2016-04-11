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

#ifndef EPIC_IMPORT_CONTEXT_HPP
#define EPIC_IMPORT_CONTEXT_HPP

#include "import/Parser.hpp"
#include <map>
#include <ostream>
#include <sstream>

#include <stddef.h>
#include <string.h>

namespace Epic
{
    namespace Import
    {
        class Context
        {
        public:
            typedef boost::spirit::classic::file_iterator<char>     iterator_t;     // file iterator for text files
            typedef boost::spirit::classic::parse_info<iterator_t>  parse_info_t;   // scanner context for boost::spirit
            typedef LineInfo<iterator_t>                            line_info_t;
            // parser context for boost::spirit

            struct Error
            {
                size_t m_line;
                size_t m_column;
                Error(size_t line, size_t col) : m_line(line), m_column(col)
                {
                }
                operator std::string()
                {
                    std::ostringstream ss("Error");
                    ss << " on line: "  << m_line;
                    ss << " near col: " << m_column;
                    return ss.str();
                }
            };


        private:
            iterator_t              m_begin;
            iterator_t              m_header;
            iterator_t              m_end;
            line_info_t             m_line_info;
            parse_info_t            m_parse_info;
            str_vector_t            m_row;
            str_vector_t            m_fldnames;

            bool
            next()
            {
                bool ret = (false == error());
                if(ret)
                    m_begin = m_parse_info.stop;
                return ret;
            }

            bool
            header()
            {
                m_parse_info = parse_csv(m_begin,m_end,m_fldnames,m_line_info);
                return next();
            }

            bool
            fieldset()
            {
                m_parse_info = parse_csv(m_begin,m_end,m_row,m_line_info);
                return next();
            }

            size_t
            line() const
            {
                return m_line_info.m_line_number;
            }

            size_t
            column_index() const
            {
                return std::distance(m_line_info.m_line_iter, m_parse_info.stop);
            }

            size_t
            column_number() const
            {
                return ( column_index() + 1 ) ;
            }

            bool
            error()
            {
                return (!m_parse_info.hit);
            }

            std::string &
            error_message( std::string & s)
            {
                Error err(line(),column_number());
                s = err;
                return s;
            }
        public:

            explicit Context(iterator_t begin, iterator_t end) :
                m_begin(begin), m_header(m_begin), m_end(end), m_line_info(m_begin)
            {
            }

            ~Context()
            {
            }

            bool more_rows()
            {
                return (m_begin != m_end);
            }

            bool read_row(str_vector_t & row)
            {
                bool rc = fieldset();
                if(rc)
                {
                    row.swap(m_row);
                    m_row.clear();
                }
                return rc;
            }

            template <class T>
            bool
            load(T & functor)
            {
                if(m_begin == m_header && m_begin != m_end)
                {
                    std::string s;
                    if(!header())
                    {
                        functor(error_message(s));
                        return false;
                    }

                    // allow caller to cancel
                    if(!functor(m_fldnames))
                        return false;

                    while(m_begin != m_end)
                    {
                        if(!fieldset())
                        {
                            functor(error_message(s));
                            return false;
                        }

                        // allow caller to cancel
                        if(!functor(line(),m_row))
                            return false;

                        m_row.clear();
                    }
                    return true;
                }
                return false;
            }
        }; // Context

    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_CONTEXT_HPP */
