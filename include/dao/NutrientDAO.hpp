#ifndef EPIC_DAO_NUTRIENT_DAO_HPP
#define EPIC_DAO_NUTRIENT_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO 
    {   
        class Nutrient; //forward declaration
    } // DAO namespace

    namespace NutrientDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO nutrients (code,description,units) VALUES (?,?,?) ;"),
                m_find_by_code("SELECT id,description,units FROM nutrients WHERE code = ? ;"),
                m_find_by_id("SELECT code,description,units FROM nutrients WHERE id = ? ;") { }


            // find a nutrient given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Nutrient & nutrient);

            // find a nutrient given a code
            bool find_by_code(int code, Epic::DAO::Nutrient & nutrient);

            // save a nutrient
            bool save(Epic::DAO::Nutrient & nutrient);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_code;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        // find a nutrient given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Nutrient & nutrient);

        // find a nutrient given a code
        bool find_by_code(int code, Epic::DAO::Nutrient & nutrient);

        // save a nutrient
        bool save(Epic::DAO::Nutrient & nutrient);

    } // NutrientDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_NUTRIENT_DAO_HPP */
