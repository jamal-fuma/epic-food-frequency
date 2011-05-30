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

#ifndef EPIC_DAO_NUTRIENT_HPP
#define EPIC_DAO_NUTRIENT_HPP

#include <vector>

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
                
                // load the model from file
                static bool load();
                static bool load(const std::string & filename);
  
                // find all nutrients
                static bool find_all(std::vector<Epic::DAO::Nutrient> & nutrients);
               
                // persist to db
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
                                  << "]" ;
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
