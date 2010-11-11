#ifndef EPIC_IMPORT_HPP
#define EPIC_IMPORT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include "dataset/Database.hpp"
#include "import/Parser.hpp"
#include "import/Context.hpp"
#include "logging/Logger.hpp"

namespace Epic
{
    namespace Import
    {
        template <class Data>
        bool
        load(std::string & filename, Data & data)
        {
            Context::iterator_t first(filename);
            if(!first)
                return false;

            Context ctx(first,first.make_end());
            return ctx.load(data);
        }

        struct DBModel
        {
            DBModel() {}

            virtual bool error(std::string & error_message)
            {  
                std::cerr << error_message << std::endl;
                return false;
            }

            virtual ~DBModel(){}

            protected:
                virtual bool header(str_vector_t & names) = 0;
                virtual bool body(size_t line, str_vector_t & values) = 0;              
            
            public:
                static bool
                same_header( const std::string  & name, 
                        const std::vector< std::string > & expected, 
                        const std::vector< std::string > & actual)
                {
                    std::ostringstream ss;

                    // sufficent fields for format
                    if(actual.size() < expected.size())
                    {
                        ss << "Unexpected number of fields in " << name 
                            << " import file," << " expected "   << expected.size() << " field(s)" 
                            << " got " << actual.size() << " field(s)" << std::endl;

                        Epic::Logging::error(ss.str());
                        return false;
                    }

                    std::vector< std::string >::const_iterator it,end;
                    it = expected.begin(); end = expected.end();

                    for(size_t pos=0; it != end; ++pos,++it)
                    {
                        if(*it != actual.at(pos))
                        {
                            ss << "Unexpected fieldname for field: " << (pos+1) << " of " << name 
                                << " import file," << " expected "   << *it 
                                << " got " << actual.at(pos) << std::endl;

                            Epic::Logging::error(ss.str());
                            return false;
                        }
                    }
                    return true;
                }

        };

        template<class T>
        struct ImportModel
        {
            T m_model;
            Database::Transaction m_transaction;
            ImportModel() {}

            // header
            bool operator()(str_vector_t & v) { return m_model.header(v); }

            // line
            bool operator()(size_t line, str_vector_t & v) { return m_model.body(line,v); }
            
            // error
            bool 
            operator()(std::string & error_message) { return m_model.error(error_message) ; }

            bool
            commit()
            {
                return m_transaction.commit();
            }
        };
        
         
    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_HPP */


