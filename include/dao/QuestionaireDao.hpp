#ifndef TEST_QUESTIONAIRE_DAO_HPP
#define TEST_QUESTIONAIRE_DAO_HPP

#include "Epic_lib.hpp"

namespace Test
{
    namespace Questionaire
    {
        class Questionaire; //forward declaration

        class DataAccess
        {
        public:
            DataAccess() : 
                m_insert("INSERT INTO questionaires(filename) VALUES (?) ;"),
                m_find_by_filename("SELECT id FROM questionaires WHERE filename = ? ;"),
                m_find_by_id("SELECT filename FROM questionaires WHERE id = ? ;") { }


            // find a questionaire given an id
            bool find(sqlite3_int64 id, Questionaire & questionaire);

            // find a questionaire given a filename
            bool find(const std::string & filename, Questionaire & questionaire);

            // save a questionaire
            bool save(Questionaire & questionaire);

        private:
            Epic::Database::PreparedStatement m_insert;
            Epic::Database::PreparedStatement m_find_by_filename;
            Epic::Database::PreparedStatement m_find_by_id;
        };
        
        bool find_by_filename(const std::string & filename, Questionaire & questionaire);
        bool find_by_id(sqlite3_int64, Questionaire & questionaire);
        bool save(Questionaire & questionaire);
    } // Questionaire namespace

} // Test namespace

#endif /* ndef TEST_QUESTIONAIRE_DAO_HPP */
