#ifndef EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class FrequencyInsertStatement : public PreparedStatement
        {
        public:
            FrequencyInsertStatement() : 
                PreparedStatement("INSERT INTO frequencies (code,amount) VALUES (?,?) ;")
            {
            }

            void
            bind(sqlite3_int64 frequency, double multiplier)
            {
		bind_int(1,frequency);
                bind_double(2,multiplier);
            }
        };

    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_FREQUENCY_INSERT_STATEMENT_HPP */
