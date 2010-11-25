#ifndef EPIC_DAO_MEAL_HPP
#define EPIC_DAO_MEAL_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Meal 
        {
            public:
                Meal() : m_id(0), m_valid(false) {}

                explicit Meal(const std::string & name, const std::string & description) :
                    m_valid(false), 
                    m_id(0),
                    m_name(name),
                    m_description(description) {}

                static Meal find_by_name(const std::string & name) ;
                static Meal find_by_id(sqlite3_int64 id) ;

                static bool load(const std::string & filename);
                bool save();

                // accessors
                sqlite3_int64 get_id() const { 
                    return m_id; 
                }

                std::string get_name() const { 
                    return m_name; 
                }

                std::string get_description() const { 
                    return m_description; 
                }

                bool name_empty() const { 
                    return m_name.empty() ; 
                }
                
                bool description_empty() const { 
                    return m_description.empty() ; 
                }

                // mutators

                void set_id(sqlite3_int64 id) { 
                    m_id = id; 
                }
 
                void set_name(const std::string & name) { 
                    m_name = name; 
                }

                void set_description(const std::string & description) { 
                    m_description = description; 
                }

                bool validate() {
                    m_valid = ( (m_id > 0) && (!name_empty() && !description_empty()));
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Meal & meal) {
                    rhs << "Meal: [" << meal.get_id()               << ",'" 
                                         << meal.get_name()         << "','"
                                         << meal.get_description()  << "'"
                                  << "]" << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64   m_id;
                std::string     m_name;
                std::string     m_description;
                bool            m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_MEAL_HPP */
