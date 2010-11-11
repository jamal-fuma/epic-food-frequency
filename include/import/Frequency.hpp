
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
                // sufficent fields for format
                if(v.size() < Frequency::fields)
                {
                    std::cerr << "Unexpected number of fields in frequencies import file";
                    std::cerr << " expected at least " << Frequency::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }


                if(v[0] != "CODE")
                {
                    std::cerr << "Unexpected field in frequencies import file";
                    std::cerr << " expected first field to be 'FREQUENCY_CODE' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "VALUE")
                {
                    std::cerr << "Unexpected field in frequencies import file";
                    std::cerr << " expected second field to be 'VALUE' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }

                return true;
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
