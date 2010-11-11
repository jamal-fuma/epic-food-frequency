#ifndef EPIC_IMPORT_QUESTIONAIRE_HPP
#define EPIC_IMPORT_QUESTIONAIRE_HPP

#include "import/Import.hpp"
#include "dataset/Database.hpp"
#include "dataset/RespondentStatement.hpp"
#include "dataset/ResponseStatement.hpp"
#include "dataset/QuestionStatement.hpp"

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

            QuestionaireDataModel() { }       
            bool header(str_vector_t & names);
            bool body(size_t line, str_vector_t & values);
        };

        struct QuestionaireData : ImportModel<QuestionaireDataModel>
        {
            QuestionaireData(){}
        };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_QUESTIONAIRE_HPP */
