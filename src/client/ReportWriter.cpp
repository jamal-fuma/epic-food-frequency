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

#include "ReportWriter.hpp"
#include "libhelper/Logger.hpp"
#include "libdao/Database.hpp"

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
                    out << m_person_it->get_reference() << ","
                        << nut_it->get_meal_id() << ","
                        << std::setprecision(6) << std::fixed <<  totals[nut_it->get_meal_id()] << ","
                        << nut_it->get_nutrient_code() << ","
                        << std::setprecision(6) << std::fixed <<  nut_it->get_quantity()
                        << "\n";
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
