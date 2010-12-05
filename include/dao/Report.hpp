#ifndef EPIC_DAO_REPORT_HPP
#define EPIC_DAO_REPORT_HPP

#include "Epic_lib.hpp"
#include <vector>

namespace Epic
{
    namespace DAO
    {
        class Report 
        {
            public:
                Report() : m_meal_id(0), m_food_id(0), m_amount(0) {}
                
                // query temporary table find all report lines given an person id
                static bool find_all(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports);

                // create the temporary table
                static void create();

                // drop the temporary table
                static void destroy();

                // acesssors

                sqlite3_int64 get_meal_id() const {
                    return m_meal_id;
                }
                
                sqlite3_int64 get_food_id() const {
                    return m_food_id;
                }
                
                double get_amount() const {
                    return m_amount;
                }


                // mutators

                void set_meal_id(sqlite3_int64 meal_id) { 
                    m_meal_id = meal_id; 
                }
            
                void set_food_id(sqlite3_int64 food_id) { 
                    m_food_id = food_id; 
                }
                
                void set_amount(double amount) { 
                    m_amount = amount;
                }

            private:
                sqlite3_int64 m_meal_id;
                sqlite3_int64 m_food_id;
                double        m_amount;
        };
    } // DAO namespace

} // Epic namespace


#endif /* ndef EPIC_DAO_REPORT_HPP */
