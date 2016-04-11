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

#include "ReportDAO.hpp"
#include "dao/Report.hpp"

// create a temporary table to speed up processing
void Epic::ReportDAO::DataAccess::create()
{
    Epic::Database::execute(m_create);
    Epic::Database::PreparedStatement find_all("SELECT meal_id,food_id,amount FROM person_foods_tmp WHERE person_id = ? ;");

    m_find_all = find_all;
}


// drop all the data that's been processed
void Epic::ReportDAO::DataAccess::destroy()
{
    Epic::Database::execute(m_destroy);
    m_find_all = Epic::Database::PreparedStatement();
}

// find report data given a person_id
bool Epic::ReportDAO::DataAccess::find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports)
{
    if(SQLITE_OK != m_find_all.bind_int64(1,person_id) )
    {
        Epic::Logging::Error().log() << "Binding person_id with value: " << person_id << " failed: DB error was: " << Epic::Database::last_error() ;
        return false;
    }

    std::vector<Epic::DAO::Report> tmp;
    tmp.swap(reports);
    for(int rc = m_find_all.step(); (SQLITE_ROW == rc); rc = m_find_all.step())
    {
        Epic::DAO::Report report;
        report.set_meal_id(m_find_all.column_int64(0));
        report.set_food_id(m_find_all.column_int64(1));
        report.set_amount(m_find_all.column_double(2));
        reports.push_back(report);
    }
    m_find_all.reset();
    return (reports.size() > 0 );
}

// singleton versions

// create a temporary table to speed up processing
void Epic::ReportDAO::create()
{
   Epic::Pattern::Singleton< Epic::ReportDAO::DataAccess >::instance().create();
}


// drop the temporary table
void Epic::ReportDAO::destroy()
{
    Epic::Pattern::Singleton< Epic::ReportDAO::DataAccess >::instance().destroy();
}

// find all data for given person
bool Epic::ReportDAO::find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports)
{
    return Epic::Pattern::Singleton< Epic::ReportDAO::DataAccess >::instance().find_all_by_person_id(person_id,reports);
}

void Epic::DAO::Report::create()
{
    Epic::ReportDAO::create();
}

// find all data for given person
bool Epic::DAO::Report::find_all(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports)
{
    return Epic::ReportDAO::find_all_by_person_id(person_id,reports);
}

void Epic::DAO::Report::destroy()
{
    Epic::ReportDAO::destroy();
}
