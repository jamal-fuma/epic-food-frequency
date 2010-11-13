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

