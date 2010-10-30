#ifndef EPIC_DATABASE_RESULTS_QUERY_STATEMENT_HPP
#define EPIC_DATABASE_RESULTS_QUERY_STATEMENT_HPP

#include "../include/import/Import.hpp"
#include "../include/dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        struct ResultSet
        {
            Epic::Import::str_vector_t m_columns;
            std::string                m_delim;

            const std::string & ffq_line() const { 
                return m_columns.at(0) ; 
            }
            const std::string & food_code() const { 
                return m_columns.at(1) ; 
            }
            const std::string & nutrient_code() const { 
                return m_columns.at(2) ; 
            }
            const std::string & nutrient_quantity() const { 
                return m_columns.at(3) ; 
            }

            ResultSet(const std::string & delim=",") : m_delim(delim)
            {
            }

            friend std::ostream & operator <<(std::ostream & out, const ResultSet & rs)
            {
                out << rs.ffq_line()          << rs.m_delim;
                out << rs.food_code()         << rs.m_delim;
                out << rs.nutrient_code()     << rs.m_delim;
                out << rs.nutrient_quantity() << rs.m_delim;
                out << std::endl;
                return out;
            }
        };


        class ResultQueryStatement
        {
            bool      m_bound;
            Statement m_sql;
        public:
            ResultQueryStatement(DBConnection & db) : 
                m_bound(false),
	        m_sql(db,
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
               	sqlite3_bind_int64(m_sql, 1, respondent);
                m_bound = true;
            }

            int
            step(ResultSet & dest)
            {
                if(!m_bound)
                {
                    throw std::runtime_error("Need to bind statement prior to exec");
                }

	        int rc = sqlite3_step(m_sql);
                if(SQLITE_ROW == rc)
                {
                    dest.m_columns.clear();
                    dest.m_columns.push_back(reinterpret_cast<const char *>(sqlite3_column_text(m_sql,0) ));
                    dest.m_columns.push_back(reinterpret_cast<const char *>(sqlite3_column_text(m_sql,1)  ));
                    dest.m_columns.push_back(reinterpret_cast<const char *>(sqlite3_column_text(m_sql,2)));
                    dest.m_columns.push_back(reinterpret_cast<const char *>(sqlite3_column_text(m_sql,3)));
                }
                return rc;
            }

            void
            reset()
            {
                if(m_bound)
                {
                    m_sql.reset();
                    m_bound = false;
                }
            }
        };


    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_RESULTS_QUERY_STATEMENT_HPP */

