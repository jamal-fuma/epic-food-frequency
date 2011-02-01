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

#ifndef EPIC_DATABASE_REPORT_HPP
#define EPIC_DATABASE_REPORT_HPP

#include "dataset/ResultStatement.hpp"
namespace Epic
{
    namespace Database
    {
        class Report
        {
            Database::ResultQueryStatement        m_result;
            Database::ResultSet                   m_rs;

            public:
                Report() {}

            bool
            list(std::ostream & out);

            bool
            respondents(std::ostream & out)
            {
                Database::Statement sql ("select reference,id from respondents");

                int rc;
                for( rc = sql.step(); (SQLITE_ROW == rc); rc = sql.step() )
                {
             //       std::string     reference = sql.column_text(0);
            //        sqlite3_int64   ref_id    = sql.column_int(1);
       //             out << reference << " " << ref_id << std::endl;
                    response(out,sql);
                }
                return (SQLITE_DONE == rc);
            }

            void
            response(std::ostream & out, Database::Statement & sql)
            {
                // grab each respondent in turn
                std::string     reference = sql.column_text(0);
                sqlite3_int64   ref_id    = sql.column_int(1);

                // handle responses for this respondent
                m_result.bind(ref_id);
                int rc;
                while(SQLITE_ROW == (rc = m_result.step(m_rs)))
                {
                    out << reference  << "," << m_rs ;
                }
                m_result.reset();
            }
        };
    } // Epic::Database
} // Epic
#endif /* ndef EPIC_DATABASE_REPORT_HPP */

