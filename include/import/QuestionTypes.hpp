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
                str_vector_t expected;
                expected.push_back("ID");
                expected.push_back("TITLE");
                return Epic::Import::DBModel::same_header("question_types",expected,v);
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
