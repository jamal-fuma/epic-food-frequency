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

#ifndef EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class FoodInsertStatement : public PreparedStatement
        {
        public:
            FoodInsertStatement() :
                PreparedStatement("INSERT INTO foods (code,nutrient,amount) VALUES (?,?,?) ;")
            {
            }

            void
            bind(const std::string & food_code, sqlite3_int64 nutrient_code, double quantity)
            {
	        bind_text(1,food_code);
	        bind_int(2,nutrient_code);
                bind_double(3,quantity);
            }
        };

    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_FOOD_INSERT_STATEMENT_HPP */
