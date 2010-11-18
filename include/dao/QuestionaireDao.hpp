#ifndef EPIC_DAO_QUESTIONAIRE_DAO_HPP
#define EPIC_DAO_QUESTIONAIRE_DAO_HPP

#include "Epic_lib.hpp"

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
                m_find_by_filename("SELECT id FROM questionaires WHERE filename = ? ;"),
                m_find_by_id("SELECT filename FROM questionaires WHERE id = ? ;") { }


            // find a questionaire given an id
            bool find(sqlite3_int64 id, Epic::DAO::Questionaire & questionaire);

            // find a questionaire given a filename
            bool find(const std::string & filename, Epic::DAO::Questionaire & questionaire);

            // save a questionaire
            bool save(Epic::DAO::Questionaire & questionaire);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_filename;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        bool find_by_filename(const std::string & filename, Epic::DAO::Questionaire & questionaire);
        bool find_by_id(sqlite3_int64, Epic::DAO::Questionaire & questionaire);
        bool save(Epic::DAO::Questionaire & questionaire);
    } // QuestionaireDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_QUESTIONAIRE_DAO_HPP */
