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

#ifndef EPIC_DAO_CEREAL_DAO_HPP
#define EPIC_DAO_CEREAL_DAO_HPP

#include "libsqlite/sqlite3.h"
#include "Statement.hpp"

namespace Epic
{
    namespace DAO
    {
        class Cereal; // forward declarations

    } // Epic::DAO namespace

    namespace CerealDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO cereals (food_id,weight_id) VALUES (?,?) ;"),
                m_find_by_food_id("SELECT id,weight_id FROM cereals WHERE food_id = ? ;"),
                m_find_by_id("SELECT food_id,weight_id FROM cereals WHERE id = ? ;") { }

            // find a cereal given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Cereal & cereal);

            // find a cereal given a food_id
            bool find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Cereal & cereal);

            // save a cereal
            bool save(Epic::DAO::Cereal & cereal);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_food_id;
            Epic::Database::PreparedStatement m_find_by_id;
        };

        // find a cereal given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Cereal & cereal);

        // find a cereal given a food_id
        bool find_by_food_id(sqlite3_int64 food_id, Epic::DAO::Cereal & cereal);

        // save a cereal
        bool save(Epic::DAO::Cereal & cereal);

    } // CerealDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_CEREAL_DAO_HPP */
