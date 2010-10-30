#ifndef EPIC_IMPORT_QUESTIONAIRE_HPP
#define EPIC_IMPORT_QUESTIONAIRE_HPP

#include "../include/import/Import.hpp"
#include "../include/dataset/Database.hpp"
#include "../include/dataset/RespondentStatement.hpp"
#include "../include/dataset/ResponseStatement.hpp"
#include "../include/dataset/ResultStatement.hpp"
#include "../include/dataset/QuestionStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct AbstractData
        {
            Database::Transaction               m_transaction;
            AbstractData(Database::DBConnection & db) :
                m_transaction(db) {}
        
            bool
            commit()
            {
                return m_transaction.commit();
            }
        };

        struct DBModel
        {
            Database::DBConnection              & m_db; 
            DBModel(Database::DBConnection & db) :
                m_db(db) {}

            const Database::DBConnection & database() const { return m_db ; }
 
            bool error(std::string & error_message)
            {  
                std::cerr << error_message << std::endl;
                return false;
            }

        };

        struct QuestionaireDataModel : public DBModel
        {
            // map fldno to fldname - map fldno to db id of value - map fldname to pos
            std::map<size_t,std::string>    fields_by_pos;
            std::map<size_t,sqlite3_int64>  question_ids_by_pos;
            std::map<std::string,size_t>    pos_by_field;

            sqlite3_int64                   m_questionaire_id;

            Database::RespondentInsertStatement   m_respondent;
            Database::ResponseInsertStatement     m_response;
            Database::ResultQueryStatement        m_result;
            Database::ResultSet                   m_rs;
            Database::QuestionInsertStatement     m_qs;
            QuestionaireDataModel(Database::DBConnection & db) :
                DBModel(db),
                m_respondent(db),
                m_response(db),
                m_result(db),
                m_qs(db)
            {
            }       

            bool header(str_vector_t & names);
            bool body(size_t line, str_vector_t & values);
         };

        struct QuestionaireData
        {
            QuestionaireDataModel m_model;
            Import::AbstractData m_importer; 
            QuestionaireData(Database::DBConnection & db) :
                m_model(db), 
                m_importer(db)
            {
            }

            // header
            bool operator()(str_vector_t & v) { return m_model.header(v); }

            // line
            bool operator()(size_t line, str_vector_t & v) { return m_model.body(line,v); }
            
            // error
            bool 
            operator()(std::string & error_message) { return m_model.error(error_message) ; }

            bool commit(){ return m_importer.commit(); }
          };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_QUESTIONAIRE_HPP */
