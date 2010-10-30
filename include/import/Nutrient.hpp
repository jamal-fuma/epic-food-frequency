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
            Database::DBConnection            & m_db;
            Database::NutrientInsertStatement   m_ns; 
            Database::Transaction               m_transaction;

            NutrientData(Database::DBConnection & db) :
                m_db(db), 
                m_ns(db),
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
                if(v.size() < Nutrient::fields)
                {
                    std::cerr << "Unexpected number of fields in nutrients import file";
                    std::cerr << " expected at least " << Nutrient::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }


                if(v[0] != "CODE")
                {
                    std::cerr << "Unexpected field in nutrients import file";
                    std::cerr << " expected first field to be 'CODE' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "DESCRIPTION")
                {
                    std::cerr << "Unexpected field in nutrients import file";
                    std::cerr << " expected second field to be 'DESCRIPTION' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }

                if(v[2] != "UNITS")
                {
                    std::cerr << "Unexpected field in nutrients import file";
                    std::cerr << " expected third field to be 'UNITS' ";
                    std::cerr << " got '" << v[2] << "'" << std::endl;
                    return false;
                }
                return true;
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
