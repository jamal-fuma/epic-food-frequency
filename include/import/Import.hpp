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

#ifndef EPIC_IMPORT_HPP
#define EPIC_IMPORT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include "import/Parser.hpp"
#include "import/Context.hpp"

namespace Epic
{
    namespace Import
    {
        template <class Data>
        bool
        load(std::string & filename, Data & data)
        {
            Context::iterator_t first(filename);
            if(!first)
                return false;

            Context ctx(first,first.make_end());
            return ctx.load(data);
        }

    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_HPP */
