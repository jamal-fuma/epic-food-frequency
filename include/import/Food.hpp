#ifndef EPIC_IMPORT_FOODS_HPP
#define EPIC_IMPORT_FOODS_HPP

#include "import/Import.hpp"
#include "dataset/FoodStatement.hpp"

namespace Epic
{
    namespace Import
    {
        struct Food
        {
            static const size_t fields = 3;
        };

        struct FoodData
        {
            Database::DBConnection            & m_db;
            Database::FoodInsertStatement       m_statement; 
            Database::Transaction               m_transaction;

            FoodData(Database::DBConnection & db) :
                m_db(db), 
                m_statement(db),
                m_transaction(m_db)
            {
                
            }

            bool
            operator()(std::string & error_message)
            {
                std::cerr << error_message << std::endl;
                return false;
            }

            // header
            bool operator()(str_vector_t & v)
            {
                // sufficent fields for format
                if(v.size() < Food::fields)
                {
                    std::cerr << "Unexpected number of fields in foods import file";
                    std::cerr << " expected at least " << Food::fields;
                    std::cerr << " got " << v.size() << " fields" << std::endl;
                    return false;
                }


                if(v[0] != "FOOD_CODE")
                {
                    std::cerr << "Unexpected field in foods import file";
                    std::cerr << " expected first field to be 'FOOD_CODE' ";
                    std::cerr << " got '" << v[0] << "'" << std::endl;
                    return false;
                }
        
                if(v[1] != "NUTRIENT")
                {
                    std::cerr << "Unexpected field in foods import file";
                    std::cerr << " expected second field to be 'NUTRIENT' ";
                    std::cerr << " got '" << v[1] << "'" << std::endl;
                    return false;
                }

                if(v[2] != "QUANTITY")
                {
                    std::cerr << "Unexpected field in foods import file";
                    std::cerr << " expected third field to be 'QUANTITY' ";
                    std::cerr << " got '" << v[2] << "'" << std::endl;
                    return false;
                }
                return true;
            }

            // data lines
            bool
            operator()(size_t line, str_vector_t & v)
            {
                if(v.size() == Food::fields)
                {
                    // nutrient code
                    Conversion::IntegerString nutrient_id(v[1]);
                    long id = nutrient_id;

                    // quantity
                    Conversion::NutrientQuantity nutrient_quantity(v[2]);
                    double quantity = nutrient_quantity;

                    // food code is just a string
                    m_statement.bind(v[0],id,quantity);
                    m_statement.step();
                    m_statement.reset();
                }
                else
                {
                    std::cerr << "Error in foods import file: ";
                    std::cerr << "skipping line :" << line;
                }
                return true;
            }
            
            bool
            commit()
            {
                return m_transaction.commit();
            }
        };

    } // Epic::Import
} // Epic

#endif /* ndef EPIC_IMPORT_FOODS_HPP */
