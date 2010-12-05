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
