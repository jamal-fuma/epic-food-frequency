#ifndef EPIC_DAO_FOOD_DAO_HPP
#define EPIC_DAO_FOOD_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO 
    {   
        class Food; //forward declaration
    } // DAO namespace

    namespace FoodDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO foods (name,description) VALUES (?,?) ;"),
                m_find_by_name("SELECT id,description FROM foods WHERE name = ? ;"),
                m_find_by_id("SELECT name,description FROM foods WHERE id = ? ;") { }


            // find a food given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Food & food);

            // find a food given a name
            bool find_by_name(const std::string & name, Epic::DAO::Food & food);

            // save a food
            bool save(Epic::DAO::Food & food);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_name;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        // find a food given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Food & food);

        // find a food given a name
        bool find_by_name(const std::string & name, Epic::DAO::Food & food);

        // save a food
        bool save(Epic::DAO::Food & food);

    } // FoodDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_FOOD_DAO_HPP */
