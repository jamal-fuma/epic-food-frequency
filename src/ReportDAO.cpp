#include "dao/ReportDAO.hpp"
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

// find a report given a name
bool Epic::ReportDAO::DataAccess::find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports)
{

    if(SQLITE_OK != m_find_all.bind_int64(1,person_id) )
    {
        std::ostringstream ss;
        ss << "Binding person_id with value: " << person_id << " failed\n" ;
        ss << "DB error was: " << Epic::Database::last_error() << "\n";
        Epic::Logging::error(ss.str());
        throw std::runtime_error(ss.str());
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

void Epic::DAO::Report::destroy()
{   
    Epic::ReportDAO::destroy();
}

// find all data for given person
bool Epic::DAO::Report::find_all(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports)
{
    return Epic::ReportDAO::find_all_by_person_id(person_id,reports);
}
