#ifndef TEST_PERSON_DAO_HPP
#define TEST_PERSON_DAO_HPP

#include "Epic_lib.hpp"

namespace Test
{
    namespace Person
    {
        class Person; //forward declaration

        class DataAccess
        {
        public:
            DataAccess() : 
                m_insert("INSERT INTO people(reference) VALUES (?) ;"),
                m_find_by_reference("SELECT id FROM people WHERE reference = ? ;"),
                m_find_by_id("SELECT reference FROM people WHERE id = ? ;") { }


            // find a person given an id
            bool find(sqlite3_int64 id, Person & person);

            // find a person given a reference
            bool find(const std::string & reference, Person & person);

            // save a person
            bool save(Person & person);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_reference;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        bool find_by_reference(const std::string & reference, Person & person);
        bool find_by_id(sqlite3_int64, Person & person);
        bool save(Person & person);
    } // Person namespace

} // Test namespace

#endif /* ndef TEST_PERSON_DAO_HPP */
