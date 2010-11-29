#ifndef EPIC_DAO_PORTION_DAO_HPP
#define EPIC_DAO_PORTION_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Portion; //forward declaration
    } // DAO namespace

    namespace PortionDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO portions (amount) VALUES (?) ;"),
                m_find_by_id("SELECT amount FROM portions WHERE id = ? ;") { }

            // find a portion given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion);

            // save a portion
            bool save(Epic::DAO::Portion & portion);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_id;
        };

        // find a portion given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Portion & portion);

        // save a portion
        bool save(Epic::DAO::Portion & portion);

    } // PortionDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_PORTION_DAO_HPP */

