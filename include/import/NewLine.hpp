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

#ifndef EPIC_IMPORT_NEWLINE_HPP
#define EPIC_IMPORT_NEWLINE_HPP

#include "../include/import/LineInfo.hpp"

namespace Epic
{
    namespace Import
    {
         // wrapper that updates the line information
        // whenever an newline is consumed by parser
        template<typename IteratorT>
        struct NewLine
        {
            LineInfo<IteratorT> & m_line_info; // reference to newline counter

            NewLine( LineInfo<IteratorT>& info ) :
                m_line_info(info)
            {
            }

            // called whenever an newline is consumed by parser
            void
            operator()(
                    IteratorT begin, // unused
                    IteratorT end
                    ) const
            {
                // update line counter and iterator
                m_line_info(end);
            }
        };

    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_NEWLINE_HPP */
