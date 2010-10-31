#ifndef EPIC_DATABASE_REPORT_HPP
#define EPIC_DATABASE_REPORT_HPP

#include "../include/dataset/ResultStatement.hpp"
namespace Epic
{
    namespace Database
    {
        class Report
        {
            Database::ResultQueryStatement        m_result;
            Database::ResultSet                   m_rs;
            Database::DBConnection &              m_db;
            
            public:
                Report(Database::DBConnection & db) :
                    m_result(db),m_db(db) { }

            bool
            list(std::ostream & out, sqlite3_int64 respondent_id)
            {
                // grab questionaire respondent external id
                Database::Statement m_sql (m_db,"select reference from respondents where id = ?");
                sqlite3_bind_int64(m_sql, 1, respondent_id);

                // walk over the responses
                int rc = sqlite3_step(m_sql); 
                for( ; (SQLITE_ROW == rc); rc = sqlite3_step(m_sql) )
                {
                    std::string reference = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,0) );
                    m_result.bind(respondent_id);

                    // display results
                    while(SQLITE_ROW == m_result.step(m_rs))
                    {
                        out << reference << "," << m_rs;
                        // v.at(pos_by_field["ID"]) << "," <<
                    }
                    m_result.reset();
                }
                return (SQLITE_DONE == rc);
            }

            void
            list_all(std::ostream & out)
            {
                // grab questionaire respondent internal id
                Database::Statement m_sql (m_db,"select id from respondents;");
                int rc = sqlite3_step(m_sql);
                std::vector<sqlite3_int64> respondents;
                for( ; (SQLITE_ROW == rc); rc = sqlite3_step(m_sql) )
                {
                    respondents.push_back(sqlite3_column_int64(m_sql,0));
                }
                m_sql.reset();
                std::vector<sqlite3_int64>::const_iterator  begin,end;
                begin   = respondents.begin();
                end     = respondents.end();
                for(; begin != end; ++begin)
                {
                    list(out,*begin);
                }
            }
        };
    } // Epic::Database
} // Epic
#endif /* ndef EPIC_DATABASE_REPORT_HPP */

