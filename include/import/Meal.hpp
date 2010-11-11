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
                str_vector_t expected;
                expected.push_back("LINE NUMBER");
                expected.push_back("FFQNAME");
                expected.push_back("SGDESC");
                return Epic::Import::DBModel::same_header("meals",expected,v);
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
