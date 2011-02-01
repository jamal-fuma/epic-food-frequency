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

#ifndef EPIC_IMPORT_NUTRIENTS_HPP
#define EPIC_IMPORT_NUTRIENTS_HPP

#include "import/Import.hpp"
#include "dataset/NutrientStatement.hpp"
#include "conversion/Conversion.hpp"

namespace Epic
{
    namespace Import
    {
        struct Nutrient
        {
            static const size_t fields = 3;
        };

        struct NutrientData
        {
            Database::NutrientInsertStatement   m_ns;
            Database::Transaction               m_transaction;

            NutrientData()
            {

            }

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
                expected.push_back("DESCRIPTION");
                expected.push_back("UNITS");
                return Epic::Import::DBModel::same_header("nutrients",expected,v);
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Nutrient::fields)
                {
                    Conversion::IntegerString ns_id(v[0]);
                    long id = ns_id;
                    m_ns.bind(id,v[1],v[2]);
                    m_ns.step();
                    m_ns.reset();
                }
                else
                {
                    std::cerr << "Error in nutrients import file: ";
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

#endif /* ndef EPIC_IMPORT_NUTRIENTS_HPP */
