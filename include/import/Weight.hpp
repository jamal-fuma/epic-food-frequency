#ifndef EPIC_IMPORT_WEIGHT_HPP
#define EPIC_IMPORT_WEIGHT_HPP

#include "import/Import.hpp"
#include "dataset/WeightStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Weight
        {
            static const size_t fields = 2;
        };

        struct WeightData
        {
            Database::WeightInsertStatement     m_statement; 
            Database::Transaction               m_transaction;

            WeightData() { }

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
                return Epic::Import::DBModel::same_header("weights",expected,v);
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Weight::fields)
                {
                  // weight code
                    Conversion::IntegerString weight_code(v[0]);
                    long code = weight_code;

                    // quantity
                    Conversion::DecimalString weight_quantity(v[1]);
                    double quantity = weight_quantity;

                    m_statement.bind(code,quantity);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in weights import file: ";
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

#endif /* EPIC_IMPORT_WEIGHT_HPP */

