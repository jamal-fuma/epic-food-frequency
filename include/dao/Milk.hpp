#ifndef EPIC_DAO_MILK_HPP
#define EPIC_DAO_MILK_HPP

#include "Epic_lib.hpp"
#include <iomanip>

namespace Epic
{
    namespace DAO
    {
        class Milk
        {
            public:
                Milk() :  m_id(0), 
                            m_food_id(0),
                            m_code(0),
                            m_valid(false) {}

                static Milk find_by_id(sqlite3_int64 id) ;
                static Milk find_by_code(int code) ;
                static Milk find_by_food_id(sqlite3_int64 food_id) ;

                static bool load(const std::string & filename);

                bool save();

                sqlite3_int64 get_id() const {
                    return m_id;
                }
                
                sqlite3_int64 get_food_id() const {
                    return m_food_id;
                }
                
                int get_code() const {
                    return m_code;
                }

                // mutators

                void set_id(sqlite3_int64 id) {
                    m_id = id;
                }
                
                void set_food_id(sqlite3_int64 food_id) {
                    m_food_id = food_id;
                }

                void set_code(int code) {
                    m_code = code;
                }

                bool validate() {
                    m_valid = ( (m_id > 0 && m_food_id > 0 && m_code > 0) );
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Milk & milk) {
                    rhs << "Milk: ["  << milk.get_id()           << ","
                                        << milk.get_food_id()      << ","
                                        << milk.get_code()
                                << "]"  << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64   m_id;
                sqlite3_int64   m_food_id;
                int             m_code;
                bool            m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_MILK_HPP */
