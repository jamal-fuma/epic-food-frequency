/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EPIC_DAO_WEIGHT_HPP
#define EPIC_DAO_WEIGHT_HPP


#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

#include "libsqlite/sqlite3.h"

namespace Epic
{
    namespace DAO
    {
        class Weight
        {
            public:
                Weight() : m_id(0), m_amount(0.0F), m_valid(false) {}

                static Weight find_by_id(sqlite3_int64 id) ;
                static bool load();
                static bool load(const std::string & filename);

                // find the upper and lower bounds of the weight range
                static bool find_bounds(sqlite3_int64 & upper, sqlite3_int64 & lower);

                // find all weights
                static bool find_all(std::vector<Epic::DAO::Weight> & weights);


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
