#include "Epic_lib.hpp"
#include "imp.hpp"

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

struct response
{
    sqlite3_int64   person_id;
    sqlite3_int64   meal_id;
    sqlite3_int64   frequency_id;
};


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
    
    bool preload()
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
                std::ostringstream ss;
                ss << "Unable to load nutrients for food_code: " << food_it->get_name() << ": " << food_it->get_description() << "\n";
                Epic::Logging::error(ss.str());
            }
        }
        
        // find nutrients 
        if(!Epic::DAO::Nutrient::find_all(m_nutrients))
        {
            Epic::Logging::error("Unable to load nutrients from db\n");
            return false;
        }

        return true;
    }

    virtual ~ReportWriter() 
    {
        // drop all the data that's been processed
        Epic::DAO::Report::destroy();
    }
 
    bool food_report(std::ostream & out)
    {
        std::ostringstream ss;
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

                  ss << "Processing completed for Respondent: " << m_person_it->get_reference() << "\n";
                  Epic::Logging::note(ss.str());
                  ss.str(std::string());
            }
        }
        m_person_it = begin;
        return true;
    }
  
    bool meal_report(std::ostream & out)
    {
        std::ostringstream ss;

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
                      Epic::DAO::Food food = m_foods.at(report_it->get_food_id() -1);
                      food_nutrient_map_iterator_t map_itr = m_nutrients_by_food_id.find(food.get_id());
                      
                      // acculate food_code totals
                      totals[report_it->get_meal_id()]  += report_it->get_amount();

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
                
                ss << "Processing completed for Respondent: " << m_person_it->get_reference() << "\n";
                Epic::Logging::note(ss.str());
                ss.str(std::string());
            }
        }
        m_person_it = begin;
        return true;
    }
    
    bool nutrient_report(std::ostream & out)
    {
        std::ostringstream ss;

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
                      for(std::vector<Epic::DAO::MealNutrient>::const_iterator nut_it = meal_nutrients.begin(), nut_end = meal_nutrients.end(); nut_it != nut_end; ++nut_it)
                      {
                          out << m_person_it->get_reference() << "," << nut_it->get_nutrient_code() << "," << std::setprecision(6) << std::fixed <<  totals[nut_it->get_nutrient_code()] << "\n";
                      }
                      Epic::DAO::MealNutrient::clear();
                      tr_2.commit();
                  }
                
                ss << "Processing completed for Respondent: " << m_person_it->get_reference() << "\n";
                Epic::Logging::note(ss.str());
                ss.str(std::string());
            }
        }
        m_person_it = begin;
        return true;
    }

    bool nutrient_wide_report(std::ostream & out)
    {
        std::ostringstream ss;

        if(m_person_it != m_end)
            write_nutrient_wide_header(out);
        
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
                
                ss << "Processing completed for Respondent: " << m_person_it->get_reference() << "\n";
                Epic::Logging::note(ss.str());
                ss.str(std::string());
            }
        }
        m_person_it = begin;
        return true;
    }

    std::ostream & write_food_header(std::ostream & out)
    {
        out << "ID,MEAL_ID,FOOD_CODE,FOOD_PORTION,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
        return out;
    }
 
    std::ostream & write_meal_header(std::ostream & out)
    {
        out << "ID,MEAL_ID,MEAL_PORTION,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
        return out;
    }
 
    std::ostream & write_nutrient_header(std::ostream & out)
    {
        out << "ID,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
        return out;
    }
 
    std::ostream & write_nutrient_wide_header(std::ostream & out)
    {
        out << "ID,";
        
        for(std::vector<Epic::DAO::Nutrient>::const_iterator nut_it = m_nutrients.begin(), nut_end = m_nutrients.end(); nut_it != nut_end; )
        {
            out << "NUTRIENT_" << nut_it->get_code() ;
            out << ((++nut_it == nut_end) ? "\n" : "," ) ;
        }
        return out;
    }

    std::ostream & write_food_line(  std::ostream & out, 
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
    
    bool store_nutrients(sqlite3_int64 meal_id, double amount, food_nutrient_iterator_t nutrient_it, food_nutrient_iterator_t nutrient_end)
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


private:
    std::vector<Epic::DAO::Report> m_reports;
    std::map< sqlite3_int64, std::vector<Epic::DAO::FoodNutrient> > m_nutrients_by_food_id;
    std::vector<Epic::DAO::Food> m_foods;
    std::vector<Epic::DAO::Nutrient> m_nutrients;
    person_iterator_t m_person_it;
    person_iterator_t m_end;
};



int
main(int argc, char **argv)
{
    std::string conf    = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";
    std::string schema  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/sql/model.sql" ;
    std::string dbase   = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/test/foods.db" ;
    std::string import  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/test/test.csv" ;

    Epic::Logging::open("error.log");
    if(!Epic::Config::load(conf))
    {
        std::ostringstream ss;
        ss << "Config file missing " << conf << std::endl;
        Epic::Logging::error(ss.str());
        return EXIT_FAILURE;
    }

    // overwrite the schema variable listed in config file with the new version
    if(!Epic::Config::insert("schema",schema,true))
        return EXIT_FAILURE;

    // overwrite the database variable listed in config file with the new version
    if(!Epic::Config::insert("database",dbase,true))
        return EXIT_FAILURE;

    Epic::Database::connect();

    Epic::Import::CSVReader rdr;

    if(argc > 1)
    {
        import = argv[1] ;
    }
    
    if(!rdr.open(import))
    {
        return EXIT_FAILURE;
    }
    

    std::ostringstream ss;

    Epic::DAO::Questionaire questionaire;
    questionaire.set_filename(import);
    
    if(!questionaire.save())
    {
        ss << "Questionaire: " << questionaire.get_filename() << " cant be saved to db ";
        ss << "skipping remainder of input" << std::endl; 
        Epic::Logging::error(ss.str());
        ss.str(std::string());
        return EXIT_FAILURE;
    }

    Epic::Import::str_vector_t v,h;

    Epic::Config::Config cnf;

    
    std::vector<response> responses;
    sqlite3_int64 frequency_upper,frequency_lower;
    sqlite3_int64 weight_upper,weight_lower;
    
    Epic::DAO::Frequency::find_bounds(frequency_upper,frequency_lower);
    Epic::DAO::Weight::find_bounds(weight_upper,weight_lower);

    // find all meals
    std::vector<Epic::DAO::Meal> meals; 
    if(!Epic::DAO::Meal::find_all(meals))
    {
        ss << "Unable to load meals from db\n";
        Epic::Logging::error(ss.str());
        ss.str(std::string());
        return EXIT_FAILURE;
    }
  
    // find all weights
    std::vector<Epic::DAO::Weight> weights;
    if(!Epic::DAO::Weight::find_all(weights))
    {
        ss << "Unable to load weights from db\n";
        Epic::Logging::error(ss.str());
        ss.str(std::string());
        return EXIT_FAILURE;
    }
    
    // find all frequencies
    std::vector<Epic::DAO::Frequency> frequencies;
    if(!Epic::DAO::Frequency::find_all(frequencies))
    {
        ss << "Unable to load frequencies from db\n";
        Epic::Logging::error(ss.str());
        ss.str(std::string());
        return EXIT_FAILURE;
    }
 

    
    Epic::Database::Transaction tr;
    
    std::vector<sqlite3_int64> people_ids;

    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                h = v;
                continue;
            }
            Epic::Import::str_vector_t::size_type size = v.size();
            for(Epic::Import::str_vector_t::size_type pos=0; pos != size; ++pos)
            {
                // wire in alternative fields names here
                cnf.insert(h[pos],v[pos],true);
            }

            Epic::DAO::Person person;
            std::string value;

            if(cnf.find("ID",value) )
            {
                person.set_reference(value);
                if(!person.save())
                {
                    ss << "Respondent: " << person.get_reference() << " cant be saved to db ";
                    ss << "skipping remainder of input" << std::endl; 
                    Epic::Logging::error(ss.str());
                    ss.str(std::string());
                    continue;
                }
                questionaire.attach(person);
            }

            // find all the meal frequencies
            for( std::vector<Epic::DAO::Meal>::const_iterator ci = meals.begin(),end = meals.end(); ci != end; ++ci)
            {
                if(cnf.find(ci->get_name(),value))
                {
                    // need to validate frequency 
                    sqlite3_int64 frequency_id   = Epic::Conversion::IntegerString(value);
                    if(frequency_id >= frequency_lower && frequency_id <= frequency_upper)
                    {
                        // find the frequency object matching this id
                        person.attach_meal(*ci,frequencies.at(frequency_id -1));
                    }
                    else
                    {
                        ss << "Respondent: " << person.get_reference() << " supplied ";
                        ss << "invalid frequency:  " << value << " for meal: " << ci->get_name() << std::endl; 
                        Epic::Logging::error(ss.str());
                        ss.str(std::string());
                    }
                }
            }

            // visible fat
            if(cnf.find("VISIBLE_FAT",value) )
            {
                sqlite3_int64 weight_id = Epic::Conversion::IntegerString(value);
                // find the weight object matching this id
                if(weight_id >= weight_lower && weight_id <= weight_upper)
                {
                    person.attach_visible_fat(weights.at(weight_id-1));
                }
                else
                {
                    ss << "Respondent: " << person.get_reference() << " supplied ";
                    ss << "invalid weight:  " << value << " for visible fat" << std::endl; 
                    Epic::Logging::error(ss.str());
                    ss.str(std::string());

                    // default weighting 
                }
            }

            if(cnf.find("CERIAL_FOOD",value) )
            {
                if(value == "")
                {
                    ss << "Respondent: " << person.get_reference() << " supplied ";
                    ss << "no cereals:  " << std::endl; 
                    Epic::Logging::error(ss.str());
                    ss.str(std::string());
                    // default cereal
                }
                else
                {
                    std::vector< std::string > food_codes;
                    Epic::Util::Token(value).split(';').values(food_codes);
                    std::vector< std::string >::const_iterator it, it_end = food_codes.end();
                    for(it = food_codes.begin(); it != it_end; ++it)
                    {
                        ss << "searching for cereal with food_code of '" << *it << "'\n" ;
                        Epic::Logging::note(ss.str());
                        ss.str(std::string());

                        person.attach_cereal(
                                Epic::DAO::Cereal::find_by_food_id(
                                    Epic::DAO::Food::find_by_name(*it).get_id()
                                    )
                                );
                    }
                }
            }

            // frying fat codes
            if(cnf.find("FAT_FRYING_FOOD",value) )
            {
                if(value == "")
                {
                    ss << "Respondent: " << person.get_reference() << " supplied ";
                    ss << "no frying fats:  " << std::endl; 
                    Epic::Logging::error(ss.str());
                    ss.str(std::string());
                    // default frying fat
                }
                else
                {
                    std::vector< std::string > food_codes;
                    Epic::Util::Token(value).split(';').values(food_codes);
                    std::vector< std::string >::const_iterator it, it_end = food_codes.end();
                    for(it = food_codes.begin(); it != it_end; ++it)
                    {
                        person.attach_frying_fat( Epic::DAO::Food::find_by_name(*it) );
                    }
                }
            }

            // baking fat codes
            if(cnf.find("FAT_BAKING_FOOD",value) )
            {
                if(value == "")
                {
                    ss << "Respondent: " << person.get_reference() << " supplied ";
                    ss << "no baking fats:  " << std::endl; 
                    Epic::Logging::error(ss.str());
                    ss.str(std::string());
                    // default baking fat
                }
                else
                {
                    std::vector< std::string > food_codes;
                    Epic::Util::Token(value).split(';').values(food_codes);
                    std::vector< std::string >::const_iterator it, it_end = food_codes.end();
                    for(it = food_codes.begin(); it != it_end; ++it)
                    {
                        person.attach_baking_fat( Epic::DAO::Food::find_by_name(*it) );
                    }
                }
            }
        }
    }
    tr.commit();

    std::vector<Epic::DAO::Person> people;
    
    if(questionaire.find_people(people))
    {
        ReportWriter meal_wtr(people.begin(),people.end());
        if(meal_wtr.preload())
        {
            std::ofstream m_file("meals_report.csv");
            if(!m_file.is_open())
            {
                std::cerr << "unable to open output file " << std::endl;
                return EXIT_FAILURE;
            }
            meal_wtr.meal_report(m_file);
            m_file.close();
            
            std::ofstream f_file("food_report.csv");
            if(!f_file.is_open())
            {
                std::cerr << "unable to open output file " << std::endl;
                return EXIT_FAILURE;
            }
            meal_wtr.food_report(f_file);
            f_file.close();

            std::ofstream n_file("nutrient_report.csv");
            if(!n_file.is_open())
            {
                std::cerr << "unable to open output file " << std::endl;
                return EXIT_FAILURE;
            }
            meal_wtr.nutrient_report(n_file);
            n_file.close();

            std::ofstream w_file("nutrient_wide_report.csv");
            if(!w_file.is_open())
            {
                std::cerr << "unable to open output file " << std::endl;
                return EXIT_FAILURE;
            }
            meal_wtr.nutrient_wide_report(w_file);
            w_file.close();
        }

    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}


void person_milks(Person & person, const Epic::Config::Config & cnf)
{
    std::string value;

    // milk
    if(cnf.find("MILK",value) )
    {
        person.set_milk_type(value);
    }

    if(cnf.find("MILK_FOOD",value) )
    {
        person.set_milk_food(value);
    }

    if(cnf.find("MILK_FREQUENCY",value) )
    {
        person.set_milk_portion(value);
    }

    Milk        milk       = person.get_milk();
    Cereal      cereal_1   = person.get_primary_cereal();
    Cereal      cereal_2   = person.get_secondary_cereal();
    BakingFat   bfat       = person.get_baking_fat();
    FryingFat   ffat       = person.get_frying_fat();


    std::cout << "Person      : ref (" << person.get_reference() << ")" << std::endl;
    std::cout << "Visible Fat : code (" << person.get_visible_fat() << ")" << std::endl;
    std::cout << "Milk        : type (" << milk.get_type() << ") food_code (" << milk.get_food_code() << ") portion (" << milk.get_portion() << ")" << std::endl;

    if(person.get_meal_frequency("CERIAL",value))
    {
        std::cout << "Cereal      : frequency (" << value << ")" << std::endl;
    }
    std::cout << std::endl <<std::endl;

}

