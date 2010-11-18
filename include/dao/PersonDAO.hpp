#ifndef EPIC_DAO_PERSON_DAO_HPP
#define EPIC_DAO_PERSON_DAO_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO 
    {   
        class Person; //forward declaration
    } // DAO namespace

    namespace PersonDAO
    {
        class DataAccess
        {
        public:
            DataAccess() : 
                m_insert("INSERT INTO people(reference) VALUES (?) ;"),
                m_find_by_reference("SELECT id FROM people WHERE reference = ? ;"),
                m_find_by_id("SELECT reference FROM people WHERE id = ? ;") { }


            // find a person given an id
            bool find(sqlite3_int64 id, Epic::DAO::Person & person);

            // find a person given a reference
            bool find(const std::string & reference, Epic::DAO::Person & person);

            // save a person
            bool save(Epic::DAO::Person & person);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_reference;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        bool find_by_reference(const std::string & reference, Epic::DAO::Person & person);
        bool find_by_id(sqlite3_int64, Epic::DAO::Person & person);
        bool save(Epic::DAO::Person & person);
    } // PersonDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_PERSON_DAO_HPP */
