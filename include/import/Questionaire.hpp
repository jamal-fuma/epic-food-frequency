#ifndef EPIC_IMPORT_QUESTIONAIRE_HPP
#define EPIC_IMPORT_QUESTIONAIRE_HPP

#include "../include/import/Import.hpp"
#include "../include/dataset/Database.hpp"
#include "../include/dataset/RespondentStatement.hpp"
#include "../include/dataset/ResponseStatement.hpp"
#include "../include/dataset/QuestionStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct QuestionaireDataModel : public DBModel
        {
            // map fldno to fldname - map fldno to db id of value - map fldname to pos
            std::map<size_t,std::string>    fields_by_pos;
            std::map<size_t,sqlite3_int64>  question_ids_by_pos;
            std::map<std::string,size_t>    pos_by_field;

            sqlite3_int64                   m_questionaire_id;

            Database::RespondentInsertStatement   m_respondent;
            Database::ResponseInsertStatement     m_response;

            Database::QuestionInsertStatement     m_qs;
            QuestionaireDataModel(Database::DBConnection & db) :
                DBModel(db),
                m_respondent(db),
                m_response(db),
                m_qs(db)
            {
            }       
            bool header(str_vector_t & names);
            bool body(size_t line, str_vector_t & values);
        };

        struct QuestionaireData : ImportModel<QuestionaireDataModel>
        {
            QuestionaireData(Database::DBConnection & db) :
                 ImportModel<QuestionaireDataModel>(db) {}
        };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_QUESTIONAIRE_HPP */
