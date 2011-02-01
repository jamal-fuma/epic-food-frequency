/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
