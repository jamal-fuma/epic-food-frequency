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

#ifndef EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class FrequencyInsertStatement : public PreparedStatement
        {
        public:
            FrequencyInsertStatement() :
                PreparedStatement("INSERT INTO frequencies (code,amount) VALUES (?,?) ;")
            {
            }

            void
            bind(sqlite3_int64 frequency, double multiplier)
            {
		bind_int(1,frequency);
                bind_double(2,multiplier);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP */
