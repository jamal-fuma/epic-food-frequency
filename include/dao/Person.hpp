#ifndef EPIC_DAO_PERSON_HPP
#define EPIC_DAO_PERSON_HPP

#include "Epic_lib.hpp"

#include "dao/Weight.hpp"
#include "dao/Portion.hpp"
#include "dao/Meal.hpp"
#include "dao/Frequency.hpp"
namespace Epic
{
    namespace DAO
    {
        class Food;         // forward declaration
        class Cereal;       // forward declaration
        class Milk;         // forward declaration

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

                // record meal frequencies for this person
                void process_meals( const Epic::Config::Config & cnf, 
                        const std::vector<Epic::DAO::Meal> & meals, 
                        const std::vector<Epic::DAO::Frequency> & frequencies, 
                        sqlite3_int64 frequency_upper, 
                        sqlite3_int64 frequency_lower);
                
                // record visible fat weighting for this person
                void process_visible_fat( const Epic::Config::Config & cnf, 
                            const std::vector<Epic::DAO::Weight> & weights, 
                            sqlite3_int64 weight_upper, 
                            sqlite3_int64 weight_lower);

                // record cereals for this person
                void process_cereals(const Epic::Config::Config & cnf, const std::string & default_cereal);
 
                // record baking fats for this person
                void process_baking_fats(const Epic::Config::Config & cnf, const std::string & default_fat);

                // record frying fats for this person
                void process_frying_fats(const Epic::Config::Config & cnf, const std::string & default_fat);

                // record milks for this person
                void process_milk(const Epic::Config::Config & cnf, 
                            const std::vector<Epic::DAO::Portion> & portions, 
                            sqlite3_int64 portion_upper, 
                            sqlite3_int64 portion_lower,
                            const std::string & default_milk);

            private:
                sqlite3_int64 m_id;
                std::string m_reference;
                bool m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_PERSON_HPP */
