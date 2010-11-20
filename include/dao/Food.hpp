#ifndef EPIC_DAO_FOOD_HPP
#define EPIC_DAO_FOOD_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Food 
        {
            public:
                Food() : m_id(0), m_valid(false) {}

                explicit Food(const std::string & name, const std::string & description) :
                    m_valid(false), 
                    m_id(0),
                    m_name(name),
                    m_description(description) {}

                static Food find_by_name(const std::string & name) ;
                static Food find_by_id(sqlite3_int64 id) ;

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
                    m_valid = ( (m_id > 0) && !name_empty() );
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Food & food) {
                    rhs << "Food: [" << food.get_id()               << ",'" 
                                         << food.get_name()         << "','"
                                         << food.get_description()  << "'"
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


#endif /* ndef EPIC_DAO_FOOD_HPP */
