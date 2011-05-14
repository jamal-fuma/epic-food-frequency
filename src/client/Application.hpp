#ifndef EPIC_APPLICATION_HPP
#define EPIC_APPLICATION_HPP

#include "Epic_lib.hpp"

#include "dao/Report.hpp"
#include "dao/Person.hpp"
#include "dao/Food.hpp"
#include "dao/Nutrient.hpp"
#include "dao/FoodNutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Frequency.hpp"
#include "dao/Weight.hpp"
#include "dao/Cereal.hpp"
#include "dao/Questionaire.hpp"
#include "dao/MealNutrient.hpp"

#include "config/Resource.hpp"
#include "import/Import.hpp"
#include "conversion/Conversion.hpp"


namespace Epic
{
    namespace Client
    {
        typedef std::vector<Epic::DAO::Person>::const_iterator                                  person_iterator_t;
       
        typedef std::vector<Epic::DAO::Report>::const_iterator                                  report_iterator_t;
        
        typedef std::vector<Epic::DAO::FoodNutrient>::const_iterator                            food_nutrient_iterator_t;
        
        typedef std::map< sqlite3_int64, std::vector<Epic::DAO::FoodNutrient> >::const_iterator food_nutrient_map_iterator_t;

        class Application
        {
            enum FileType { InputFile=1, JobFile} ;

        public:
            enum ReportType { Foods=1, Meals, Nutrients, Spreadsheet} ;

            Application() :
                m_status(Application::InputFile), 
                m_report_type(Application::Foods)
            {
            }

            ~Application()
            {
            }


            void
            set_input(const std::string & filename)
            {
                m_input_file = filename;
                m_status = Application::InputFile;
            }

            void
            set_jobfile(const std::string & filename)
            {
                m_input_file = filename;
                m_status = Application::JobFile;
            }
            
            void
            set_report_type(Application::ReportType type)
            {
                m_report_type = type;
            }

            void
            set_output(const std::string & filename)
            {
                m_output_file = filename;
            }

            int run();

            bool load_questionaire(Epic::DAO::Questionaire & questionaire, size_t & size);
            bool write_report(Epic::DAO::Questionaire & questionaire, std::ostream & file);
           private:

            std::string         m_input_file;
            std::string         m_output_file;
            Application::FileType m_status;
            Application::ReportType m_report_type;
            
            std::vector<Epic::DAO::Meal> m_meals; 
            std::vector<Epic::DAO::Weight> m_weights;
            std::vector<Epic::DAO::Portion> m_portions;
            std::vector<Epic::DAO::Frequency> m_frequencies;

            sqlite3_int64 m_weight_upper,m_weight_lower;
            sqlite3_int64 m_portion_upper,m_portion_lower;
            sqlite3_int64 m_frequency_upper,m_frequency_lower;
        };
    }// Epic::Client
}//Epic
#endif /* ndef EPIC_APPLICATION_HPP */
