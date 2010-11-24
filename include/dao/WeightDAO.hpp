#ifndef EPIC_DAO_WEIGHT_DAO_HPP
#define EPIC_DAO_WEIGHT_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Weight; //forward declaration
    } // DAO namespace

    namespace WeightDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO weights (amount) VALUES (?) ;"),
                m_find_by_id("SELECT amount FROM weights WHERE id = ? ;") { }

            // find a weight given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Weight & weight);

            // save a weight
            bool save(Epic::DAO::Weight & weight);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_id;
        };

        // find a weight given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Weight & weight);

        // save a weight
        bool save(Epic::DAO::Weight & weight);

    } // WeightDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_WEIGHT_DAO_HPP */

