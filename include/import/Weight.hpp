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
            Database::DBConnection            & m_db;
            Database::WeightInsertStatement     m_statement; 
            Database::Transaction               m_transaction;

            WeightData(Database::DBConnection & db) :
                m_db(db), 
                m_statement(db),
                m_transaction(m_db)
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
                // sufficent fields for format
                if(v.size() != Weight::fields)
                {
                    std::cerr << "Unexpected number of fields in weights import file";
                    std::cerr << " expected at least " << Weight::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }

                if(v[0] != "CODE")
                {
                    std::cerr << "Unexpected field in weights import file";
                    std::cerr << " expected first field to be 'CODE' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "VALUE")
                {
                    std::cerr << "Unexpected field in weights import file";
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

