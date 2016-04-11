/* This file is part of the libEpic client */

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

#ifndef EPIC_REPORT_WRITER_HPP
#define EPIC_REPORT_WRITER_HPP

#if defined( __cplusplus)

#include "Application.hpp"

namespace Epic
{
    namespace Client
    {
        class ReportWriter
        {
            public:
                ReportWriter(person_iterator_t begin, person_iterator_t end) : m_person_it(begin), m_end(end)
            {
                Epic::DAO::Report::create();
            }
                ~ReportWriter()
                {
                    // drop all the data that's been processed
                    Epic::DAO::Report::destroy();
                }

                bool preload();

                // most detailed
                std::ostream & write_food_header(std::ostream & out);
                bool food_report(std::ostream & out);

                // summarise food_report
                std::ostream & write_meal_header(std::ostream & out);
                bool meal_report(std::ostream & out);


                // summarise meal_report
                std::ostream & write_nutrient_header(std::ostream & out);
                bool nutrient_report(std::ostream & out);

                // pretty printed nutrient_report
                std::ostream & write_spreadsheet_header(std::ostream & out);
                bool spreadsheet_report(std::ostream & out);

            private:
                std::vector<Epic::DAO::Report> m_reports;
                std::map< sqlite3_int64,
                       std::vector<Epic::DAO::FoodNutrient> > m_nutrients_by_food_id;
                std::vector<Epic::DAO::Food> m_foods;
                std::vector<Epic::DAO::Nutrient> m_nutrients;
                person_iterator_t m_person_it;
                person_iterator_t m_end;


                std::ostream & write_food_line(  std::ostream & out,
                        person_iterator_t person_it,
                        sqlite3_int64 meal_id,
                        const Epic::DAO::Food & food,
                        double amount,
                        food_nutrient_iterator_t nutrient_it,
                        food_nutrient_iterator_t nutrient_end);


                bool store_nutrients(
                        sqlite3_int64 meal_id,
                        double amount,
                        food_nutrient_iterator_t nutrient_it,
                        food_nutrient_iterator_t nutrient_end);
        };

    }// Epic::Client
}//Epic

#endif /* defined( __cplusplus) */

#endif /* ndef EPIC_REPORT_WRITER_HPP */
