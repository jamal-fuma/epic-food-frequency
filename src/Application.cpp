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
#include "client/ReportWriter.hpp"

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



