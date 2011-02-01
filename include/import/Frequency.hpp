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


#ifndef EPIC_IMPORT_FREQUENCIES_HPP
#define EPIC_IMPORT_FREQUENCIES_HPP

#include "import/Import.hpp"
#include "dataset/FrequencyStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Frequency
        {
            static const size_t fields = 2;
        };

        struct FrequencyData
        {
            Database::Transaction               m_transaction;
            Database::FrequencyInsertStatement  m_statement;

            FrequencyData() { }

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
                expected.push_back("VALUE");
                return Epic::Import::DBModel::same_header("frequencies",expected,v);
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Frequency::fields)
                {
                    // frequency code
                    Conversion::IntegerString frequency_code(v[0]);
                    long code = frequency_code;

                    // quantity
                    Conversion::DecimalString frequency_quantity(v[1]);
                    double quantity = frequency_quantity;

                    m_statement.bind(code,quantity);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in frequencies import file: ";
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

#endif /* ndef EPIC_IMPORT_FREQUENCIES_HPP */
