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

        class ReportWriter
        {
            public:
                ReportWriter(person_iterator_t begin, person_iterator_t end) : m_person_it(begin), m_end(end) 
            {
                Epic::DAO::Report::create();
            }
                ~ReportWriter() 
                {
                    // drop all the data that's been processed
                    Epic::DAO::Report::destroy();
                }

                bool preload();
                
                // most detailed
                std::ostream & write_food_header(std::ostream & out);
                bool food_report(std::ostream & out);
                
                // summarise food_report
                std::ostream & write_meal_header(std::ostream & out);
                bool meal_report(std::ostream & out);
         

                // summarise meal_report
                std::ostream & write_nutrient_header(std::ostream & out);
                bool nutrient_report(std::ostream & out);

                // pretty printed nutrient_report
                std::ostream & write_spreadsheet_header(std::ostream & out);
                bool spreadsheet_report(std::ostream & out);

            private:
                std::vector<Epic::DAO::Report> m_reports;
                std::map< sqlite3_int64, 
                       std::vector<Epic::DAO::FoodNutrient> > m_nutrients_by_food_id;
                std::vector<Epic::DAO::Food> m_foods;
                std::vector<Epic::DAO::Nutrient> m_nutrients;
                person_iterator_t m_person_it;
                person_iterator_t m_end;


                std::ostream & write_food_line(  std::ostream & out, 
                        person_iterator_t person_it,
                        sqlite3_int64 meal_id, 
                        const Epic::DAO::Food & food,
                        double amount, 
                        food_nutrient_iterator_t nutrient_it,
                        food_nutrient_iterator_t nutrient_end);


                bool store_nutrients(
                        sqlite3_int64 meal_id, 
                        double amount, 
                        food_nutrient_iterator_t nutrient_it, 
                        food_nutrient_iterator_t nutrient_end);
        };

        class Application
        {
            enum FileType { InputFile=1, JobFile} ;
            enum ReportType { Foods=1, Meals, Nutrients, Spreadsheet} ;

        public:
            Application() :
                m_status(Application::InputFile), 
                m_report_type(Application::Foods)
            {
                Logging::Trace().log() << "Starting Up";
            }

            ~Application()
            {
                Logging::Trace().log() << "Shutting Down";
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
