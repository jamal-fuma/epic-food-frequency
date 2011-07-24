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

#ifndef EPIC_DAO_QUESTIONAIRE_DAO_HPP
#define EPIC_DAO_QUESTIONAIRE_DAO_HPP

#include "dao/Person.hpp"
#include "libsqlite/sqlite3.h"
#include "Statement.hpp"

#include <vector>
namespace Epic
{
    namespace DAO
    {
         class Questionaire; //forward declaration
    } // DAO namespace

    namespace QuestionaireDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                m_insert("INSERT INTO questionaires(filename) VALUES (?) ;"),
                m_attach("INSERT INTO questionaire_people (questionaire_id,person_id) VALUES(?,?) ;"),
                m_find_people_by_questionaire_id("SELECT person_id FROM questionaire_people WHERE questionaire_id = ? ;"),
                m_find_by_filename("SELECT id FROM questionaires WHERE filename = ? ;"),
                m_find_by_id("SELECT filename FROM questionaires WHERE id = ? ;") { }


            // find a questionaire given an id
            bool find_by_id(sqlite3_int64 id, Epic::DAO::Questionaire & questionaire);

            // find a questionaire given a filename
            bool find_by_filename(const std::string & filename, Epic::DAO::Questionaire & questionaire);

            // save a questionaire
            bool save(Epic::DAO::Questionaire & questionaire);

            // associate a person with a questionaire
            bool attach(const Epic::DAO::Questionaire & questionaire, const Epic::DAO::Person & person) ;

            // find all people associated with a questionaire
            bool find_people(const Epic::DAO::Questionaire & questionaire, std::vector<Epic::DAO::Person> & people);


        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_attach;
            Epic::Database::PreparedStatement m_find_by_filename;
            Epic::Database::PreparedStatement m_find_people_by_questionaire_id;
            Epic::Database::PreparedStatement m_find_by_id;
        };

        bool find_by_filename(const std::string & filename, Epic::DAO::Questionaire & questionaire);
        bool find_by_id(sqlite3_int64, Epic::DAO::Questionaire & questionaire);
        bool save(Epic::DAO::Questionaire & questionaire);

        // associate a person with a questionaire
        bool attach(const Epic::DAO::Questionaire & questionaire, const Epic::DAO::Person & person);

        // find all people associated with a questionaire
        bool find_people(const Epic::DAO::Questionaire & questionaire, std::vector<Epic::DAO::Person> & people);

  } // QuestionaireDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_QUESTIONAIRE_DAO_HPP */
