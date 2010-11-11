#ifndef EPIC_IMPORT_QUESTION_TYPES_HPP
#define EPIC_IMPORT_QUESTION_TYPES_HPP

#include "import/Import.hpp"
#include "conversion/Conversion.hpp"
#include "dataset/QuestionTypes.hpp"

namespace Epic
{
    namespace Import
    {
        struct QuestionType
        {
            static const size_t fields = 2;
        };

        struct QuestionTypeData
        {
            Database::Transaction                   m_transaction;
            Database::QuestionTypesInsertStatement  m_statement; 

            QuestionTypeData() { }

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
                if(v.size() < QuestionType::fields)
                {
                    std::cerr << "Unexpected number of fields in question types import file";
                    std::cerr << " expected at least " << QuestionType::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }


                if(v[0] != "ID")
                {
                    std::cerr << "Unexpected field in question types import file";
                    std::cerr << " expected first field to be 'ID' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "TITLE")
                {
                    std::cerr << "Unexpected field in question types import file";
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
                if(v.size() == QuestionType::fields)
                {
                    Conversion::IntegerString ns_id(v[0]);
                    long id = ns_id;
                    m_statement.bind(id,v[1]);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in question types import file: ";
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

#endif /* ndef EPIC_IMPORT_QUESTION_TYPES_HPP */
