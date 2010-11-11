#ifndef EPIC_DATABASE_WEIGHT_INSERT_STATEMENT_HPP
#define EPIC_DATABASE_WEIGHT_INSERT_STATEMENT_HPP

#include "dataset/Statement.hpp"
namespace Epic
{
    namespace Database
    {
        class WeightInsertStatement : public PreparedStatement
        {
        public:
            WeightInsertStatement() :
                PreparedStatement("INSERT INTO weights (code,amount) VALUES (?,?) ;") { }

            void
            bind(sqlite3_int64 code, double multiplier)
            {
                bind_int(1,code);
                bind_double(2,multiplier);
            }
        };

    } // Epic::Database
    
} // Epic

#endif /* ndef EPIC_DATABASE_WEIGHT_INSERT_STATEMENT_HPP */
