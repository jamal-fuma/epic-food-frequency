#ifndef TEST_PERSON_HPP
#define TEST_PERSON_HPP

#include "Epic_lib.hpp"

namespace Test
{
    namespace Person
    {
        class Person 
        {
            public:
                Person() : m_id(0), m_valid(false) {}

                explicit Person(const std::string & reference) :
                    m_id(0),m_valid(false), m_reference(reference) {}

                static Person find_by_reference(const std::string & reference) ;
                static Person find_by_id(sqlite3_int64 id) ;

                bool save();

                // accessors
                std::string get_reference() const { 
                    return m_reference; 
                }
                sqlite3_int64 get_id() const { 
                    return m_id; 
                }

                bool reference_empty() const { 
                    return m_reference.empty() ; 
                }

                // mutators
                void set_reference(const std::string & reference) { 
                    m_reference = reference; 
                }

                void set_id(sqlite3_int64 id) { 
                    m_id = id; 
                }
                
                bool validate() {
                    m_valid = ( (m_id > 0) && (!reference_empty()));
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Person & person) {
                    rhs << "Person: [" << person.get_id() << ",'" << person.get_reference() << "']" << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64 m_id;
                std::string m_reference;
                bool m_valid;
        };
    } // Person namespace

} // Test namespace


#endif /* ndef TEST_PERSON_HPP */
