#ifndef EPIC_DAO_PORTION_HPP
#define EPIC_DAO_PORTION_HPP

#include "Epic_lib.hpp"
#include <iomanip>
#include <vector>

namespace Epic
{
    namespace DAO
    {
        class Portion
        {
            public:
                Portion() : m_id(0), m_amount(0.0F), m_valid(false) {}

                explicit Portion(double amount) :
                    m_id(0),
                    m_valid(false),
                    m_amount(amount) {}

                static Portion find_by_id(sqlite3_int64 id) ;
                static bool load(const std::string & filename);
                static bool load();
                // find the upper and lower bounds of the portion range
                static bool find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower);

                // find all portions
                static bool find_all(std::vector<Epic::DAO::Portion> & portions);

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

                friend std::ostream & operator<<(std::ostream & rhs, const Portion & portion) {
                    rhs << "Portion: ["  << portion.get_id()           << ","
                                        << std::setprecision(5) << std::fixed << portion.get_amount()
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


#endif /* ndef EPIC_DAO_PORTION_HPP */
