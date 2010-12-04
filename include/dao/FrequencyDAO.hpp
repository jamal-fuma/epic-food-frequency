#ifndef EPIC_DAO_FREQUENCY_DAO_HPP
#define EPIC_DAO_FREQUENCY_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Frequency; //forward declaration
    } // DAO namespace

    namespace FrequencyDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO frequencies (amount) VALUES (?) ;"),
                m_find_bounds("SELECT min(id), max(id) FROM frequencies ;"),
                m_find_by_id("SELECT amount FROM frequencies WHERE id = ? ;") { }

            // find a frequency given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Frequency & frequency);
 
            // find the upper and lower bounds of the frequency range
            bool find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower);

           // save a frequency
            bool save(Epic::DAO::Frequency & frequency);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_id;
            Epic::Database::PreparedStatement m_find_bounds;
        };

        // find a frequency given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Frequency & frequency);
        
        // find the upper and lower bounds of the frequency range
        bool find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower);

        // save a frequency
        bool save(Epic::DAO::Frequency & frequency);

    } // FrequencyDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_FREQUENCY_DAO_HPP */

