#ifndef EPIC_DAO_WEIGHT_HPP
#define EPIC_DAO_WEIGHT_HPP

#include "Epic_lib.hpp"
#include <iomanip>

namespace Epic
{
    namespace DAO
    {
        class Weight
        {
            public:
                Weight() : m_id(0), m_amount(0.0F), m_valid(false) {}

                explicit Weight(double amount) :
                    m_id(0),
                    m_valid(false),
                    m_amount(amount) {}

                static Weight find_by_id(sqlite3_int64 id) ;

                bool save();

                sqlite3_int64 get_id() const {
                    return m_id;
                }

                double get_amount() const {
                    return m_amount;
                }

                // mutators

                void set_id(sqlite3_int64 id) {
                    m_id = id;
                }

                void set_amount(double amount) {
                    m_amount = amount;
                }

                bool validate() {
                    m_valid = ( (m_id > 0) );
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Weight & weight) {
                    rhs << "Weight: ["  << weight.get_id()           << ","
                                        << std::setprecision(5) << std::fixed << weight.get_amount()
                                << "]"  << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64   m_id;
                double          m_amount;
                bool            m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_WEIGHT_HPP */
