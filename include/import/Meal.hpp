#ifndef EPIC_IMPORT_MEAL_HPP
#define EPIC_IMPORT_MEAL_HPP

#include "import/Import.hpp"
#include "conversion/Conversion.hpp"
#include "dataset/MealStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Meal
        {
            static const size_t fields = 3;
        };

        struct MealData
        {
            Database::Transaction               m_transaction;
            Database::MealInsertStatement       m_statement; 

            MealData() { }

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
                if(v.size() < Meal::fields)
                {
                    std::cerr << "Unexpected number of fields in meals import file";
                    std::cerr << " expected " << Meal::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }

                if(v[0] != "LINE NUMBER")
                {
                    std::cerr << "Unexpected field in meals import file";
                    std::cerr << " expected first field to be 'LINE NUMBER' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "FFQNAME")
                {
                    std::cerr << "Unexpected field in meals import file";
                    std::cerr << " expected second field to be 'FFQNAME' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }
               if(v[2] != "SGDESC")
                {
                    std::cerr << "Unexpected field in meals import file";
                    std::cerr << " expected second field to be 'SGDESC' ";
                    std::cerr << " got '" << v[2] << "'" << std::endl;
                    return false;
                }


                return true;
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Meal::fields)
                {
                    // line number in questionaire grid
                    Conversion::IntegerString line_code(v[0]);
                    long code = line_code;

                    m_statement.bind(code,v[1],v[2]);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in meals import file: ";
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

#endif /* ndef EPIC_IMPORT_MEAL_HPP */
