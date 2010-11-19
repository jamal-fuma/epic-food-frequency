#ifndef EPIC_DAO_MEAL_DAO_HPP
#define EPIC_DAO_MEAL_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO 
    {   
        class Meal; //forward declaration
    } // DAO namespace

    namespace MealDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO meals (name,description) VALUES (?,?) ;"),
                m_find_by_name("SELECT id,description FROM meals WHERE name = ? ;"),
                m_find_by_id("SELECT name,description FROM meals WHERE id = ? ;") { }


            // find a meal given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Meal & meal);

            // find a meal given a name
            bool find_by_name(const std::string & name, Epic::DAO::Meal & meal);

            // save a meal
            bool save(Epic::DAO::Meal & meal);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_name;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        // find a meal given an id
        bool find_by_id(sqlite3_int64 id, Epic::DAO::Meal & meal);

        // find a meal given a name
        bool find_by_name(const std::string & name, Epic::DAO::Meal & meal);

        // save a meal
        bool save(Epic::DAO::Meal & meal);

    } // MealDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_MEAL_DAO_HPP */
