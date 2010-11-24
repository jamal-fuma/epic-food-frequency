#ifndef EPIC_DAO_NUTRIENT_HPP
#define EPIC_DAO_NUTRIENT_HPP

#include "Epic_lib.hpp"

namespace Epic
{
    namespace DAO
    {
        class Nutrient 
        {
            public:
                Nutrient() : m_id(0), m_code(0), m_valid(false) {}

                explicit Nutrient(const std::string & description,const std::string & units) :
                    m_id(0),
                    m_code(0),
                    m_valid(false), 
                    m_units(units),
                    m_description(description) {}

                static Nutrient find_by_code(int code) ;
                static Nutrient find_by_id(sqlite3_int64 id) ;

                bool save();

                // accessors
                std::string get_units() const { 
                    return m_units; 
                }

                sqlite3_int64 get_id() const { 
                    return m_id; 
                }

                int get_code() const { 
                    return m_code; 
                }

                std::string get_description() const { 
                    return m_description; 
                }

                bool units_empty() const { 
                    return m_units.empty() ; 
                }
                
                bool description_empty() const { 
                    return m_description.empty() ; 
                }

                // mutators

                void set_id(sqlite3_int64 id) { 
                    m_id = id; 
                }
 
                void set_code(int code) {
                    m_code = code; 
                }

                void set_description(const std::string & description) { 
                    m_description = description; 
                }

                void set_units(const std::string & units) { 
                    m_units = units; 
                }

                bool validate() {
                    m_valid = ( (m_id > 0) && (m_code > 0) && (!units_empty()));
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Nutrient & nutrient) {
                    rhs << "Nutrient: [" << nutrient.get_id()           << "," 
                                         << nutrient.get_code()         << ",'" 
                                         << nutrient.get_description()  << "','" 
                                         << nutrient.get_units()        << "'"
                                  << "]" << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64   m_id;
                int             m_code;
                std::string     m_units;
                std::string     m_description;
                bool            m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_NUTRIENT_HPP */