#ifndef EPIC_IMPORT_MILK_TYPES_HPP
#define EPIC_IMPORT_MILK_TYPES_HPP

#include "import/Import.hpp"
#include "dataset/MilkTypesStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct MilkTypes
        {
            static const size_t fields = 3;
        };

        struct MilkTypesData
        {
            Database::Transaction                       m_transaction;
            Database::MilkTypeInsertStatement         m_statement; 

            MilkTypesData() {} 

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
                return Epic::Import::DBModel::same_header("milk_types",expected,v);
           }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == MilkTypes::fields)
                {
                  // milk type code
                    Conversion::IntegerString milk_type_code(v[0]);
                    long code = milk_type_code;

                    m_statement.bind(code,v[1],v[2]);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in milk types import file: ";
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

#endif /* EPIC_IMPORT_MILK_TYPES_HPP */

