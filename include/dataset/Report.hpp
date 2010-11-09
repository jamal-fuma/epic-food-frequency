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
            list(std::ostream & out);

            bool
            respondents(std::ostream & out)
            {
                Database::Statement sql (m_db,"select reference,id from respondents");

                int rc = sqlite3_step(sql); 
                for( ; (SQLITE_ROW == rc); rc = sqlite3_step(sql) )
                {
                    std::string     reference = reinterpret_cast<const char *>(sqlite3_column_text(sql,0) );
                    sqlite3_int64   ref_id    = sqlite3_column_int64(sql,1);
                    out << reference << " " << ref_id << std::endl;
        //            response(out,sql);
                }
                return (SQLITE_DONE == rc);
            }

            void
            response(std::ostream & out, Database::Statement & sql)
            {
                // grab each respondent in turn
                std::string     reference = reinterpret_cast<const char *>(sqlite3_column_text(sql,0) );
                sqlite3_int64   ref_id    = sqlite3_column_int64(sql,1);
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

