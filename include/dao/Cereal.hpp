#ifndef EPIC_DAO_CEREAL_HPP
#define EPIC_DAO_CEREAL_HPP

#include "Epic_lib.hpp"
#include <iomanip>

namespace Epic
{
    namespace DAO
    {
        class Cereal
        {
            public:
                Cereal() :  m_id(0), 
                            m_food_id(0),
                            m_weight_id(0),
                            m_valid(false) {}

                explicit Cereal(sqlite3_int64 food_id, sqlite3_int64 weight_id) :
                    m_id(0),
                    m_food_id(0),
                    m_weight_id(0),
                    m_valid(false) {}

                static Cereal find_by_id(sqlite3_int64 id) ;
                static Cereal find_by_food_id(sqlite3_int64 food_id) ;

                static bool load();
                static bool load(const std::string & filename);

                bool save();

                sqlite3_int64 get_id() const {
                    return m_id;
                }
                
                sqlite3_int64 get_food_id() const {
                    return m_food_id;
                }
                
                sqlite3_int64 get_weight_id() const {
                    return m_weight_id;
                }

                // mutators

                void set_id(sqlite3_int64 id) {
                    m_id = id;
                }
                
                void set_food_id(sqlite3_int64 food_id) {
                    m_food_id = food_id;
                }

                void set_weight_id(sqlite3_int64 weight_id) {
                    m_weight_id = weight_id;
                }

                bool validate() {
                    m_valid = ( (m_id > 0 && m_food_id > 0 && m_weight_id > 0) );
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Cereal & cereal) {
                    rhs << "Cereal: ["  << cereal.get_id()           << ","
                                        << cereal.get_food_id()      << ","
                                        << cereal.get_weight_id()
                                << "]"  << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64   m_id;
                sqlite3_int64   m_food_id;
                sqlite3_int64   m_weight_id;
                bool            m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_CEREAL_HPP */
