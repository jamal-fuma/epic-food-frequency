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

#ifndef EPIC_IMPORT_CEREAL_TYPES_HPP
#define EPIC_IMPORT_CEREAL_TYPES_HPP

#include "import/Import.hpp"
#include "dataset/CerealTypesStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct CerealTypes
        {
            static const size_t fields = 4;
        };

        struct CerealTypesData
        {
            Database::Transaction                       m_transaction;
            Database::CerealTypeInsertStatement         m_statement;

            CerealTypesData() { }

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
                expected.push_back("FOOD_CODE");
                expected.push_back("DESC");
                expected.push_back("WEIGHT");
                return Epic::Import::DBModel::same_header("cereal_types",expected,v);
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == CerealTypes::fields)
                {
                  // cereal type code
                    Conversion::IntegerString cereal_type_code(v[0]);
                    long code = cereal_type_code;

                    // weight
                    Conversion::IntegerString weight_str(v[3]);
                    long weight = weight_str;

                    m_statement.bind(code,v[1],v[2],weight);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in cereal types import file: ";
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

#endif /* EPIC_IMPORT_CEREAL_TYPES_HPP */

