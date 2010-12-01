#ifndef EPIC_DAO_PERSON_HPP
#define EPIC_DAO_PERSON_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Weight;       // forward declaration
        class Food;         // forward declaration
        class Cereal;       // forward declaration
        class Milk;         // forward declaration
        class Portion;      // forward declaration
        class Meal;         // forward declaration
        class Frequency;    // forward declaration

        class Person 
        {
            public:
                Person() : m_id(0), m_valid(false) {}

                explicit Person(const std::string & reference) :
                    m_id(0),m_valid(false), m_reference(reference) {}

                static Person find_by_reference(const std::string & reference) ;
                static Person find_by_id(sqlite3_int64 id) ;

                // visible fat weighting
                bool attach_visible_fat(const Epic::DAO::Weight & weight);
                
                // frying fat food code
                bool attach_frying_fat(const Epic::DAO::Food & food);

                // baking fat food code
                bool attach_baking_fat(const Epic::DAO::Food & food);

                // cereal
                bool attach_cereal(const Epic::DAO::Cereal & cereal);
                
                // associate a person with a milk food code and portion
                bool attach_milk(const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion);

                // associate a person with a meal frequency
                bool attach_meal(const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency);

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
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_PERSON_HPP */
