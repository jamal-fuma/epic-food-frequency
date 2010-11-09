#ifndef EPIC_IMPORT_HPP
#define EPIC_IMPORT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include "../include/dataset/Dataset.hpp"
#include "../include/dataset/Database.hpp"
#include "../include/import/Parser.hpp"
#include "../include/import/Context.hpp"

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
            Database::DBConnection              & m_db; 
            DBModel(Database::DBConnection & db) :
                m_db(db) {}

            const Database::DBConnection & database() const { return m_db ; }
 
            virtual bool error(std::string & error_message)
            {  
                std::cerr << error_message << std::endl;
                return false;
            }
            virtual ~DBModel(){}

            protected:
                virtual bool header(str_vector_t & names) = 0;
                virtual bool body(size_t line, str_vector_t & values) = 0;
        };

        template<class T>
        struct ImportModel
        {
            T m_model;
            Database::Transaction m_transaction;
            ImportModel(Database::DBConnection & db) :
                m_model(db), 
                m_transaction(db) {}

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


