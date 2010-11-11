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
                // sufficent fields for format
                if(v.size() != MilkTypes::fields)
                {
                    std::cerr << "Unexpected number of fields in milk types import file";
                    std::cerr << " expected at least " << MilkTypes::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }

                if(v[0] != "CODE")
                {
                    std::cerr << "Unexpected field in milk types import file";
                    std::cerr << " expected first field to be 'CODE' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "FOOD_CODE")
                {
                    std::cerr << "Unexpected field in milk types import file";
                    std::cerr << " expected second field to be 'FOOD_CODE' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }
        
                if(v[2] != "DESC")
                {
                    std::cerr << "Unexpected field in milk types import file";
                    std::cerr << " expected third field to be 'DESC' ";
                    std::cerr << " got '" << v[2] << "'" << std::endl;
                    return false;
                }

                return true;
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

