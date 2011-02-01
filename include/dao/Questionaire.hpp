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

#ifndef EPIC_DAO_QUESTIONAIRE_HPP
#define EPIC_DAO_QUESTIONAIRE_HPP

#include "Epic_lib.hpp"
#include "dao/Person.hpp"

#include <vector>
namespace Epic
{
    namespace DAO
    {
        class Questionaire 
        {
            public:
                Questionaire() : m_id(0), m_valid(false) {}

                explicit Questionaire(const std::string & filename) :
                    m_id(0),m_valid(false), m_filename(filename) {}

                static Questionaire find_by_filename(const std::string & filename) ;
                static Questionaire find_by_id(sqlite3_int64 id) ;

                bool save();
                bool attach(Person & person);
                bool find_people(std::vector<Person> & people);

                // accessors
                std::string get_filename() const { 
                    return m_filename; 
                }
                sqlite3_int64 get_id() const { 
                    return m_id; 
                }

                bool filename_empty() const { 
                    return m_filename.empty() ; 
                }

                // mutators
                void set_filename(const std::string & filename) { 
                    m_filename = filename; 
                }

                void set_id(sqlite3_int64 id) { 
                    m_id = id; 
                }
                
                bool validate() {
                    m_valid = ( (m_id > 0) && (!filename_empty()));
                    return m_valid;
                }

                bool valid() const {
                    return m_valid;
                }

                friend std::ostream & operator<<(std::ostream & rhs, const Questionaire & questionaire) {
                    rhs << "Questionaire: [" << questionaire.get_id() << ",'" << questionaire.get_filename() << "']" << std::endl;
                    return rhs;
                }

            private:
                sqlite3_int64 m_id;
                std::string m_filename;
                bool m_valid;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_QUESTIONAIRE_HPP */
