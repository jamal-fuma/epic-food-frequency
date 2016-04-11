/* This file is part of the libEpic client */

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

#include "Logger.hpp"

void Epic::Logging::error(const std::string & str)
{
    Epic::Pattern::Singleton< Logger >::instance().error(str);
}

void Epic::Logging::trace(const std::string & str)
{

    Epic::Pattern::Singleton< Logger >::instance().trace(str);
}

void Epic::Logging::note(const std::string & str)
{
    Epic::Pattern::Singleton< Logger >::instance().note(str);
}

bool Epic::Logging::open(const std::string & filename)
{
    try
    {
        Epic::Pattern::Singleton< Logger >::instance().open(filename);
        return true;
    }
    catch(...)
    {
        return false;
    }
}
