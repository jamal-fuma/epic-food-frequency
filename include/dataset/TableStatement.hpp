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

#ifndef EPIC_DATABASE_TABLE_QUERY_STATEMENT_HPP
#define EPIC_DATABASE_TABLE_QUERY_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class TableQueryStatement
        {
            Statement m_sql;
        public:
            TableQueryStatement(DBConnection & db) :
	        m_sql(db,"SELECT name FROM sqlite_master WHERE type = 'table' ORDER BY name;");
            {
            }

            int
            exec(std::vector<std::string> & tables)
            {
                int rc = SQLITE_DONE;
                while( SQLITE_ROW == (rc = sqlite3_step(m_sql)))
                {
                    tables.push_back(sqlite3_column_text(m_sql,1));
                }
                m_sql.reset();
                return rc;
            }
        };


    } // Epic::Database

} // Epic

#endif /* ndef EPIC_DATABASE_TABLE_QUERY_STATEMENT_HPP */
