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
