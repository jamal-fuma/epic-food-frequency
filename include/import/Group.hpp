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

#ifndef EPIC_IMPORT_GROUPS_HPP
#define EPIC_IMPORT_GROUPS_HPP

#include "import/Import.hpp"
#include "dataset/GroupStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Group
        {
            static const size_t fields = 2;
        };

        struct GroupData
        {
            Database::Transaction           m_transaction;
            Database::GroupInsertStatement  m_statement;

            GroupData() { }

            bool
            operator()(std::string & error_message)
            {
                std::cerr << error_message << std::endl;
                return false;
            }

            // header
            bool operator()(str_vector_t & v)
            {
                str_vector_t expected;
                expected.push_back("CODE");
                expected.push_back("GROUP");
                return Epic::Import::DBModel::same_header("groups",expected,v);
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Group::fields)
                {
                    // group code
                    Conversion::IntegerString group_code(v[0]);
                    long code = group_code;

                    // group name
                    std::string group_name(v[1]);
                    m_statement.bind(code,group_name);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in groups import file: ";
                    std::cerr << "skipping line :" << line;
                }
                return true;
            }

            bool
            commit()
            {
                return m_transaction.commit();
            }
        };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_GROUPS_HPP */
