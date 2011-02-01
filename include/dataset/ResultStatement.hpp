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

#ifndef EPIC_DATABASE_RESULTS_QUERY_STATEMENT_HPP
#define EPIC_DATABASE_RESULTS_QUERY_STATEMENT_HPP

#include "Epic_lib.hpp"
#include "import/Import.hpp"
#include "dataset/Statement.hpp"

#include <algorithm>
#include <vector>

#include <iterator>
#include <iostream>


namespace Epic
{
    namespace Database
    {
        struct ResultSet
        {
            Epic::Import::str_vector_t m_columns;
            std::string                m_delim;

            ResultSet(const std::string & delim=",") : m_delim(delim)
            {
            }

            friend std::ostream & operator <<(std::ostream & out, const ResultSet & rs)
            {
                std::ostream_iterator< std::string > output( out, rs.m_delim.c_str() );

                std::copy(rs.m_columns.begin(),rs.m_columns.end(),output);
                return out << std::endl;
            }
        };


        class ResultQueryStatement : public PreparedStatement
        {
        public:
            ResultQueryStatement() :
	        PreparedStatement(
"SELECT meals.line       AS FFQ_LINE,"
"ingredients.food        AS FOOD_CODE,"
"nutrients.code          AS NUTRIENT_CODE,"
"((frequencies.amount * ingredients.amount) * (foods.amount / 100.00)) AS NUTRIENT_QUANTITY"
" FROM responses"
" INNER JOIN questions    ON responses.question_id = questions.id"
" INNER JOIN frequencies  ON responses.value       = frequencies.code"
" INNER JOIN meals        ON meals.name            = questions.description"
" INNER JOIN ingredients  ON ingredients.meal_id   = meals.line"
" INNER JOIN foods        ON ingredients.food      = foods.code"
" INNER JOIN nutrients    ON foods.nutrient        = nutrients.code"
" WHERE NUTRIENT_QUANTITY > 0 AND responses.respondent_id = ? ;" )
            {
            }

            void
            bind(sqlite3_int64 respondent)
            {
                bind_int(1,respondent);
            }

            int
            step(ResultSet & dest)
            {
	        int rc = Epic::Database::PreparedStatement::step();
                if(SQLITE_ROW == rc)
                {
                    dest.m_columns.clear();
                    dest.m_columns.push_back(column_text(0));
                    dest.m_columns.push_back(column_text(1));
                    dest.m_columns.push_back(column_text(2));
                    dest.m_columns.push_back(column_text(3));
                }
                return rc;
            }
        };


    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_RESULTS_QUERY_STATEMENT_HPP */

