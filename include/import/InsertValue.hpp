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

#ifndef EPIC_IMPORT_INSERT_VALUE_HPP
#define EPIC_IMPORT_INSERT_VALUE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <string.h>
#include "config/Util.hpp"

 // #include "Epic_lib.hpp"
namespace Epic
{
    namespace Import
    {
        typedef std::vector<std::string> str_vector_t;

        // wrapper that appends tokens into a container
        struct InsertValue
        {
            // container of std::string
            str_vector_t & m_tokens;

            InsertValue( str_vector_t & vec ) : m_tokens(vec) { }

            // all post-processing on tokens occurs here
            template<class IteratorT>
            void
            operator()( IteratorT begin, IteratorT end) const
            {
                std::string s(begin, end);
                std::string::size_type pos(0);

                // fold a rfc4180 literal double quote ("") into
                // a single literal double quote
                pos = s.find("\"\"", pos);
                for(; pos != std::string::npos;  pos = s.find("\"\"", pos))
                {
                    ++pos; // move past first double quote character
                    s.erase(pos, 1);
                }

                // need to remove carriage returns
                pos = s.find('\r',0);
                for(; pos != std::string::npos; pos = s.find('\r',pos+1) )
                {
                    s.erase(pos,1);
                }

                // need to remove leading/trailing whitespace
                Epic::Util::trim(s);
             //   s.erase(
             //     s.begin(),
             //         std::find_if(
//                         s.begin(), s.end(), 
//                         std::not1(
//                             std::ptr_fun<int, int>(
//                                 std::isspace
//                             )
//                         )
//                     )
//             );
                m_tokens.push_back(s);
            }
        };
    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_INSERT_VALUE_HPP */
