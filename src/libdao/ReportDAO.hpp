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

#ifndef EPIC_DAO_REPORT_DAO_HPP
#define EPIC_DAO_REPORT_DAO_HPP

#include "Epic_lib.hpp"
#include "dao/Report.hpp"
#include <vector>

namespace Epic
{
    namespace ReportDAO
    {
        class DataAccess
        {
        public:
            DataAccess() :
                // create a temporary table to speed up processing
                m_create("BEGIN; "
                          "CREATE TEMPORARY TABLE person_foods_tmp AS "
                          "SELECT person_id,meal_id,food_id,amount FROM person_food_vw ; "
                          "CREATE INDEX index_person_foods_tmp_on_person_meal_food ON "
                          "person_foods_tmp(person_id,meal_id,food_id); "
                          "COMMIT;"),

                // drop all the data that's been processed
                m_destroy("BEGIN; "
                          "DELETE FROM questionaires; "
                          "DROP INDEX index_person_foods_tmp_on_person_meal_food; "
                          "DROP TABLE person_foods_tmp; "
                          "COMMIT; "
                          "VACUUM;") {}
                          
           // create the temporary table
            void create();

            // drop the temporary table
            void destroy();

            bool find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports);
            
        private:
            Epic::Database::PreparedStatement m_find_all; 
            std::string m_create;
            std::string m_destroy;
        };

        bool find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports);

         // create the temporary table
        void create();

        // drop the temporary table
        void destroy();

    } // ReportDAO namespace

} // Epic namespace

#endif /* ndef EPIC_DAO_REPORT_DAO_HPP */
