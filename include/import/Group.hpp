#ifndef EPIC_IMPORT_GROUPS_HPP
#define EPIC_IMPORT_GROUPS_HPP

#include "import/Import.hpp"
#include "dataset/GroupStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Group
        {
            static const size_t fields = 2;
        };

        struct GroupData
        {
            Database::DBConnection            & m_db;
            Database::GroupInsertStatement  m_statement; 
            Database::Transaction               m_transaction;

            GroupData(Database::DBConnection & db) :
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
                if(v.size() < Group::fields)
                {
                    std::cerr << "Unexpected number of fields in groups import file";
                    std::cerr << " expected at least " << Group::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }


                if(v[0] != "CODE")
                {
                    std::cerr << "Unexpected field in groups import file";
                    std::cerr << " expected first field to be 'CODE' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "GROUP")
                {
                    std::cerr << "Unexpected field in groups import file";
                    std::cerr << " expected second field to be 'GROUP' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }

                return true;
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Group::fields)
                {
                    // group code
                    Conversion::IntegerString group_code(v[0]);
                    long code = group_code;

                    // group name
                    std::string group_name(v[1]);
                    m_statement.bind(code,group_name);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in groups import file: ";
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

#endif /* ndef EPIC_IMPORT_GROUPS_HPP */
