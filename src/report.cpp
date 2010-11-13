#include "dataset/Report.hpp"

 bool
 Epic::Database::Report::list(std::ostream & out)
 {
#if(0)
	 Database::Statement m_sql (m_db,

             "SELECT "
             "       respondents.id, "
             "       respondents.reference, "
             "       groups.description, "
             "       questions.description, "
             "       responses.value "
             "FROM "
             "       responses,questions,respondents,groups "
             "WHERE "
             "        responses.respondent_id     = respondents.id "
             "        AND "
             "        questions.id                = responses.question_id "
             "        AND "
             "        questions.question_type_id  = groups.id;"
"SELECT meals.id, meals.name, ingredients.food, ingredients.amount FROM meals,ingredients where ingredients.meal_id = meals.line;"
             );

     // walk over the responses
     int rc = sqlite3_step(m_sql);

     for( ; (SQLITE_ROW == rc); rc = sqlite3_step(m_sql) )
     {
         sqlite3_int64   id         = sqlite3_column_int64(m_sql,0);
         std::string     reference  = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,1) );
         std::string     group      = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,2) );
         std::string     title      = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,3) );
         std::string     response   = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,4) );
     }
     return (SQLITE_DONE == rc);
#endif
	return false;
 }


