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

#ifndef EPIC_DAO_PORTION_DAO_HPP
#define EPIC_DAO_PORTION_DAO_HPP

#include "dao/Portion.hpp"
#include "libsqlite/sqlite3.h"
#include "Statement.hpp"
#include <vector>

namespace Epic
{
    namespace PortionDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO portions (amount) VALUES (?) ;"),
                m_find_all("SELECT id,amount from portions ;"),
                m_find_bounds("SELECT min(id), max(id) FROM portions ;"),
                 m_find_by_id("SELECT amount FROM portions WHERE id = ? ;") { }

            // find a portion given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion);
            // find the upper and lower bounds of the portion range
            bool find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower);
            
            // find all portions
            bool find_all(std::vector<Epic::DAO::Portion> & portions);

            // save a portion
            bool save(Epic::DAO::Portion & portion);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_all;
            Epic::Database::PreparedStatement m_find_bounds;
            Epic::Database::PreparedStatement m_find_by_id;
        };

        // find a portion given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion);

        // find the upper and lower bounds of the portion range
        bool find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower);

        // find all portions
        bool find_all(std::vector<Epic::DAO::Portion> & portions);


        // save a portion
        bool save(Epic::DAO::Portion & portion);

    } // PortionDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_PORTION_DAO_HPP */

