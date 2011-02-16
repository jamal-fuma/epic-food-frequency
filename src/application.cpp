/* This file is part of the libEpic client */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "client/Application.hpp"

int
Epic::Client::Application::run()
{
    try
    {
        // find all meals
        if(!Epic::DAO::Meal::find_all(m_meals))
        {
            Epic::Logging::Error().log()  << "Unable to load meals from db";
            return EXIT_FAILURE;
        }

        // find all weights
        if(!Epic::DAO::Weight::find_all(m_weights) || !Epic::DAO::Weight::find_bounds(m_weight_upper,m_weight_lower))
        {
            Epic::Logging::Error().log()  << "Unable to load weights from db";
            return EXIT_FAILURE;
        }
 
        // find all portions
        if(!Epic::DAO::Portion::find_all(m_portions) || !Epic::DAO::Portion::find_bounds(m_portion_upper,m_portion_lower))
        {
            Epic::Logging::Error().log()  << "Unable to load milk portions from db";
            return EXIT_FAILURE;
        }


        // find all frequencies
        if(!Epic::DAO::Frequency::find_all(m_frequencies) || !Epic::DAO::Frequency::find_bounds(m_frequency_upper,m_frequency_lower))
        {
            Epic::Logging::Error().log()  << "Unable to load frequencies from db";
            return EXIT_FAILURE;
        }

        // parse questionare data into constiuent nutrients
        if(Application::InputFile == m_status)
        {
            Epic::DAO::Questionaire questionaire;
            questionaire.set_filename(m_input_file);
            if(!questionaire.save())
            {
                Epic::Logging::Error().log()  << "Questionaire: " << questionaire.get_filename() 
                    << " cant be saved to db, skipping remainder of input" ;
                return EXIT_FAILURE;
            }
    
            size_t size=0;
            if(!load_questionaire(questionaire,size))
            {
                Epic::Logging::Error().log()  << "Questionaire: " << questionaire.get_filename() 
                    << " processing failed" ;
                return EXIT_FAILURE;
            }

            // sort out input / output
            if(m_output_file.empty())
            {
                if(!write_report(questionaire,std::cout))
                {
                    Epic::Logging::Error().log()  << "Questionaire: " << questionaire.get_filename() 
                        << " import completed successfully, but reporting failed, imported (" << size << ") respondents" ;
                    return EXIT_FAILURE;
                }
            }
            else
            {
                std::ofstream file(m_output_file.c_str());
                if(!file.is_open())
                {
                    std::cerr << "unable to open output file: " << m_output_file << std::endl;
                   Epic::Logging::Error().log()  << "Questionaire: " << questionaire.get_filename() 
                        << " import completed successfully, but reporting failed, imported (" << size << ") respondents" ;
                    return EXIT_FAILURE;
                }
                
                if(!write_report(questionaire,file))
                {
                    Epic::Logging::Error().log()  << "Questionaire: " << questionaire.get_filename() 
                        << " import completed successfully, but reporting failed, imported (" << size << ") respondents" ;
                    return EXIT_FAILURE;
                }
            }

            Epic::Logging::Note().log()  << "Questionaire: " << questionaire.get_filename() 
                << " processing completed successfully, processed(" << size << ") respondents" ;
            return EXIT_SUCCESS;
        }

        if(Application::JobFile == m_status)
        {
            // rewire jobfile processing
             std::cout << "Job file specifed as " << m_input_file << std::endl;
             std::cout << "Job file support is currently disabled, please use direct input, with -i " << m_input_file <<std::endl;
             return EXIT_FAILURE;
        }
    }
    catch(std::runtime_error & e)
    {
        Epic::Logging::error(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


bool
Epic::Client::Application::load_questionaire(Epic::DAO::Questionaire & questionaire, size_t & size)
{
    Epic::DAO::Person person;

    Epic::Import::str_vector_t values,headers;
    Epic::Config::Config cnf;
    std::string value;
   

    Epic::Import::CSVReader rdr;
    if(!rdr.open(questionaire.get_filename()))
        return false;

    Epic::Database::Transaction tr;

    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(values))
        {
            if(!line)
            {
                headers.swap(values);
                continue;
            }
            Epic::Import::str_vector_t::size_type nelem = values.size();
            for(Epic::Import::str_vector_t::size_type pos=0; pos != nelem; ++pos)
            {
                // wire in alternative fields names here
                cnf.insert(headers[pos],values[pos],true);
            }

            bool rc = cnf.find("ID",value);
            if( (rc && value == "") || !rc)
            {
                Epic::Logging::Error().log() << "Error on line: " << line << " No questionaire ID specified";
                continue;
            }
            else
            {
                person.set_reference(value);
                if(!person.save())
                {
                    Epic::Logging::Error().log() << "Respondent: " 
                        << person.get_reference() 
                        << " cant be saved to db, skipping remainder of input";
                    continue;
                }
                questionaire.attach(person);
            }


            // handle the meals
            person.process_meals(cnf,m_meals,m_frequencies,m_frequency_upper,m_frequency_lower);

            // handle visible fat
            person.process_visible_fat(cnf,m_weights,m_weight_upper, m_weight_lower);

            // cereal food codes
            person.process_cereals(cnf,"a1362");

            // frying fat codes
            person.process_frying_fats(cnf,"a7024");

            // baking fat codes
            person.process_baking_fats(cnf,"a7024");

            // milk portions
            person.process_milk(cnf,m_portions,m_portion_upper, m_portion_lower,"a2002");

            ++size;
        }
    }
    return tr.commit();
}

bool Epic::Client::Application::write_report(Epic::DAO::Questionaire & questionaire, std::ostream & file)
{
    // report in desired fashion
    std::vector<Epic::DAO::Person> people;
    if(questionaire.find_people(people))
    {
        ReportWriter report_wtr(people.begin(),people.end());
        bool rc = false;
        if(report_wtr.preload())
        {
            switch(m_report_type)
            {
                default:
                case Application::Foods:
                    rc = report_wtr.food_report(file);
                    break;

                case Application::Meals:
                    rc = report_wtr.meal_report(file);
                    break;

                case Application::Nutrients:
                    rc = report_wtr.nutrient_report(file);
                    break;

                case Application::Spreadsheet:
                    rc = report_wtr.spreadsheet_report(file);
                    break;
            }
        }
        return rc;
    }
    return false;
}

bool Epic::Client::ReportWriter::preload()
{
    // find all foods and corresponding nutrients
    if(!Epic::DAO::Food::find_all(m_foods))
    {
        Epic::Logging::error("Unable to load foods from db\n");
        return false;
    }

    // find nutrients for foods
    for( std::vector<Epic::DAO::Food>::const_iterator food_it = m_foods.begin(), food_end = m_foods.end(); food_it != food_end; ++food_it)
    {
        std::vector<Epic::DAO::FoodNutrient> nutrients;
        if(food_it->find_nutrients(nutrients))
        {
            m_nutrients_by_food_id[food_it->get_id()] = nutrients;
        }
        else
        {
           Epic::Logging::Error().log() << "Unable to load nutrients for food_code: " << food_it->get_name() << ": " << food_it->get_description() << "\n";
        }
    }

    // find nutrients 
    if(!Epic::DAO::Nutrient::find_all(m_nutrients))
    {
        Epic::Logging::Error().log() << "Unable to load nutrients from db";
        return false;
    }

    return true;
}


bool Epic::Client::ReportWriter::food_report(std::ostream & out)
{
    if(m_person_it != m_end)
        write_food_header(out);

    person_iterator_t begin = m_person_it;

    for(; m_person_it != m_end; ++m_person_it)
    {
        // load all meals, associated food_codes and portions
        if(Epic::DAO::Report::find_all(m_person_it->get_id(),m_reports))
        {
            for(report_iterator_t report_it = m_reports.begin(), report_end = m_reports.end(); report_it != report_end; ++report_it)
            {

                Epic::DAO::Food food = m_foods.at(report_it->get_food_id() -1);
                food_nutrient_map_iterator_t map_itr = m_nutrients_by_food_id.find(food.get_id());
                if(map_itr != m_nutrients_by_food_id.end())
                {
                    write_food_line(out,m_person_it,report_it->get_meal_id(),food,report_it->get_amount(),map_itr->second.begin(),map_itr->second.end());
                }
            }

            Epic::Logging::Note().log() 
                <<  "Processing completed for Respondent: " 
                << m_person_it->get_reference() ;
        }
    }
    m_person_it = begin;
    return true;
}

bool Epic::Client::ReportWriter::meal_report(std::ostream & out)
{
    if(m_person_it != m_end)
        write_meal_header(out);

    person_iterator_t begin = m_person_it;

    for(; m_person_it != m_end; ++m_person_it)
    {
        if(Epic::DAO::Report::find_all(m_person_it->get_id(),m_reports))
        {
            Epic::Database::Transaction tr;

            std::map<sqlite3_int64, double> totals;

            for(report_iterator_t report_it = m_reports.begin(), report_end = m_reports.end(); report_it != report_end; ++report_it)
            {
                Epic::Logging::Note().log() << "Respondent: "
                                             << m_person_it->get_reference()
                                             << " consumption "
                                             << *report_it;


                Epic::DAO::Food food = m_foods.at(report_it->get_food_id() -1);
                food_nutrient_map_iterator_t map_itr = m_nutrients_by_food_id.find(food.get_id());

                // acculate food_code totals
                totals[report_it->get_meal_id()]  += report_it->get_amount();

                if(map_itr != m_nutrients_by_food_id.end())
                {
                    if(!store_nutrients(report_it->get_meal_id(),report_it->get_amount(),map_itr->second.begin(),map_itr->second.end()))
                    {
                        Epic::Logging::Error().log() << "Storing nutrients failed for " << *report_it;
                        return false;
                    }
                }
            }
            tr.commit();

            std::vector<Epic::DAO::MealNutrient> meal_nutrients;
            if(Epic::DAO::MealNutrient::find_all_meal_nutrients(meal_nutrients))
            {
                Epic::Database::Transaction tr_2;
                for(std::vector<Epic::DAO::MealNutrient>::const_iterator nut_it = meal_nutrients.begin(), nut_end = meal_nutrients.end(); nut_it != nut_end; ++nut_it)
                {
                    out << m_person_it->get_reference() << "," << nut_it->get_meal_id() << "," << std::setprecision(6) << std::fixed <<  totals[nut_it->get_meal_id()] << "," << nut_it->get_nutrient_code() << "," << std::setprecision(6) << std::fixed <<  nut_it->get_quantity() << "\n";
                }
                Epic::DAO::MealNutrient::clear();
                tr_2.commit();
            }

            Epic::Logging::Note().log() 
                <<  "Processing completed for Respondent: " 
                << m_person_it->get_reference() ;
        }
    }
    m_person_it = begin;
    return true;
}


bool Epic::Client::ReportWriter::nutrient_report(std::ostream & out)
{
    if(m_person_it != m_end)
        write_nutrient_header(out);

    person_iterator_t begin = m_person_it;

    for(; m_person_it != m_end; ++m_person_it)
    {
        if(Epic::DAO::Report::find_all(m_person_it->get_id(),m_reports))
        {
            Epic::Database::Transaction tr;

            std::map<sqlite3_int64, double> totals;

            for(report_iterator_t report_it = m_reports.begin(), report_end = m_reports.end(); report_it != report_end; ++report_it)
            {
                Epic::DAO::Food food = m_foods.at(report_it->get_food_id() -1);
                food_nutrient_map_iterator_t map_itr = m_nutrients_by_food_id.find(food.get_id());

                if(map_itr != m_nutrients_by_food_id.end())
                {
                    if(!store_nutrients(report_it->get_meal_id(),report_it->get_amount(),map_itr->second.begin(),map_itr->second.end()))
                    {
                        Epic::Logging::error("Storing nutrients failed");
                        return false;
                    }
                }
            }
            tr.commit();

            std::vector<Epic::DAO::MealNutrient> meal_nutrients;
            if(Epic::DAO::MealNutrient::find_all_nutrients(meal_nutrients))
            {
                Epic::Database::Transaction tr_2;
                for(std::vector<Epic::DAO::MealNutrient>::const_iterator nut_it = meal_nutrients.begin(), nut_end = meal_nutrients.end(); nut_it != nut_end; ++nut_it)
                {
                    // acculate nutrient totals
                    totals[nut_it->get_nutrient_code()]  += nut_it->get_quantity();
                } 
                for(std::vector<Epic::DAO::MealNutrient>::const_iterator nut_it = meal_nutrients.begin(), nut_end = meal_nutrients.end(); nut_it != nut_end; ++nut_it)
                {
                    out << m_person_it->get_reference() << "," << nut_it->get_nutrient_code() << "," << std::setprecision(6) << std::fixed <<  totals[nut_it->get_nutrient_code()] << "\n";
                }
                Epic::DAO::MealNutrient::clear();
                tr_2.commit();
            }

            Epic::Logging::Note().log() 
                <<  "Processing completed for Respondent: " 
                << m_person_it->get_reference() ;
        }
    }
    m_person_it = begin;
    return true;
}


bool Epic::Client::ReportWriter::spreadsheet_report(std::ostream & out)
{
    if(m_person_it != m_end)
        write_spreadsheet_header(out);

    person_iterator_t begin = m_person_it;

    for(; m_person_it != m_end; ++m_person_it)
    {
        if(Epic::DAO::Report::find_all(m_person_it->get_id(),m_reports))
        {
            Epic::Database::Transaction tr;

            std::map<sqlite3_int64, double> totals;

            for(report_iterator_t report_it = m_reports.begin(), report_end = m_reports.end(); report_it != report_end; ++report_it)
            {
                Epic::DAO::Food food = m_foods.at(report_it->get_food_id() -1);
                food_nutrient_map_iterator_t map_itr = m_nutrients_by_food_id.find(food.get_id());

                if(map_itr != m_nutrients_by_food_id.end())
                {
                    if(!store_nutrients(report_it->get_meal_id(),report_it->get_amount(),map_itr->second.begin(),map_itr->second.end()))
                    {
                        Epic::Logging::error("Storing nutrients failed\n");
                        return false;
                    }
                }
            }
            tr.commit();

            std::vector<Epic::DAO::MealNutrient> meal_nutrients;
            if(Epic::DAO::MealNutrient::find_all_nutrients(meal_nutrients))
            {
                Epic::Database::Transaction tr_2;
                for(std::vector<Epic::DAO::MealNutrient>::const_iterator nut_it = meal_nutrients.begin(), nut_end = meal_nutrients.end(); nut_it != nut_end; ++nut_it)
                {
                    // acculate nutrient totals
                    totals[nut_it->get_nutrient_code()]  += nut_it->get_quantity();
                } 

                // transpose rows to columns
                out << m_person_it->get_reference() << ",";
                for(std::vector<Epic::DAO::Nutrient>::const_iterator nut_it = m_nutrients.begin(), nut_end = m_nutrients.end(); nut_it != nut_end; )
                {
                    out << std::setprecision(6) << std::fixed <<  totals[nut_it->get_code()] ;
                    out << ((++nut_it == nut_end) ? "\n" : "," ) ;
                }
                Epic::DAO::MealNutrient::clear();
                tr_2.commit();
            }
            Epic::Logging::Note().log() 
                <<  "Processing completed for Respondent: " 
                << m_person_it->get_reference() ;
        }
    }
    m_person_it = begin;
    return true;
}

std::ostream & Epic::Client::ReportWriter::write_food_header(std::ostream & out)
{
    out << "ID,MEAL_ID,FOOD_CODE,FOOD_PORTION,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
    return out;
}

std::ostream & Epic::Client::ReportWriter::write_meal_header(std::ostream & out)
{
    out << "ID,MEAL_ID,MEAL_PORTION,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
    return out;
}

std::ostream & Epic::Client::ReportWriter::write_nutrient_header(std::ostream & out)
{
    out << "ID,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
    return out;
}

std::ostream & Epic::Client::ReportWriter::write_spreadsheet_header(std::ostream & out)
{
    out << "ID,";

    for(std::vector<Epic::DAO::Nutrient>::const_iterator nut_it = m_nutrients.begin(), nut_end = m_nutrients.end(); nut_it != nut_end; )
    {
        out << "NUTRIENT_" << nut_it->get_code() ;
        out << ((++nut_it == nut_end) ? "\n" : "," ) ;
    }
    return out;
}

std::ostream & Epic::Client::ReportWriter::write_food_line( 
        std::ostream & out, 
        person_iterator_t person_it,
        sqlite3_int64 meal_id, 
        const Epic::DAO::Food & food,
        double amount, 
        food_nutrient_iterator_t nutrient_it,
        food_nutrient_iterator_t nutrient_end)
{
    std::ostringstream ss;
    ss << person_it->get_reference() << "," << meal_id << "," << food.get_name() << "," << amount << "," ;
    std::string line = ss.str();

    // list scaled nutrients for food
    for(; nutrient_it != nutrient_end; ++nutrient_it)
    {
        out  << line << nutrient_it->get_nutrient_code() << "," << std::setprecision(6) << std::fixed << nutrient_it->get_scaled_amount(amount)  << "\n";
    }
    return out;
}

bool Epic::Client::ReportWriter::store_nutrients(
        sqlite3_int64 meal_id, 
        double amount, 
        food_nutrient_iterator_t nutrient_it, 
        food_nutrient_iterator_t nutrient_end)
{
    Epic::DAO::MealNutrient meal_nutrient;

    meal_nutrient.set_meal_id(meal_id);

    // store scaled nutrients for food
    for(; nutrient_it != nutrient_end; ++nutrient_it)
    {
        meal_nutrient.set_nutrient_code(nutrient_it->get_nutrient_code());
        meal_nutrient.set_quantity(nutrient_it->get_scaled_amount(amount));
        if(!meal_nutrient.save())
            return false;
    }
    return true;

}




