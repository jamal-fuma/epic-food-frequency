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

#ifndef EPIC_DAO_NUTRIENT_DAO_HPP
#define EPIC_DAO_NUTRIENT_DAO_HPP

#include "Epic_lib.hpp"
#include "dao/Nutrient.hpp"
#include <vector>

namespace Epic
{
    namespace NutrientDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO nutrients (code,description,units) VALUES (?,?,?) ;"),
                m_find_by_code("SELECT id,description,units FROM nutrients WHERE code = ? ;"),
                m_find_all("SELECT id,code,description,units FROM nutrients ORDER BY code ;"),
                m_find_by_id("SELECT code,description,units FROM nutrients WHERE id = ? ;") { }

            // find a nutrient given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Nutrient & nutrient);

            // find a nutrient given a code
            bool find_by_code(int code, Epic::DAO::Nutrient & nutrient);

            // find all nutrients
            bool find_all(std::vector<Epic::DAO::Nutrient> & nutrients);

            // save a nutrient
            bool save(Epic::DAO::Nutrient & nutrient);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_code;
            Epic::Database::PreparedStatement m_find_all;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        // find a nutrient given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Nutrient & nutrient);

        // find a nutrient given a code
        bool find_by_code(int code, Epic::DAO::Nutrient & nutrient);
  
        // find all nutrients
        bool find_all(std::vector<Epic::DAO::Nutrient> & nutrients);

        // save a nutrient
        bool save(Epic::DAO::Nutrient & nutrient);

    } // NutrientDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_NUTRIENT_DAO_HPP */
