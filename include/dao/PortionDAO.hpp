#ifndef EPIC_DAO_PORTION_DAO_HPP
#define EPIC_DAO_PORTION_DAO_HPP

#include "Epic_lib.hpp"
#include "dao/Portion.hpp"
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

