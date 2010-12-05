#include "Epic_lib.hpp"
#include "imp.hpp"

#include "dao/Report.hpp"
#include "dao/Person.hpp"
#include "dao/Food.hpp"
#include "dao/FoodNutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Frequency.hpp"
#include "dao/Weight.hpp"
#include "dao/Cereal.hpp"
#include "dao/Questionaire.hpp"

#include "config/Resource.hpp"
#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

struct response
{
    sqlite3_int64   person_id;
    sqlite3_int64   meal_id;
    sqlite3_int64   frequency_id;
};


bool find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports);

typedef std::vector<Epic::DAO::Person>::const_iterator                                  person_iterator_t;
typedef std::vector<Epic::DAO::Report>::const_iterator                                  report_iterator_t;
typedef std::vector<Epic::DAO::FoodNutrient>::const_iterator                            food_nutrient_iterator_t;
typedef std::map< sqlite3_int64, std::vector<Epic::DAO::FoodNutrient> >::const_iterator food_nutrient_map_iterator_t;


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

    

    std::map<sqlite3_int64,Epic::DAO::Frequency> frequency_by_id;
    std::map<sqlite3_int64,Epic::DAO::Weight>    weight_by_id;
    std::map<std::string,Epic::DAO::Cereal>      cereal_by_food_code;
    
    // find all meals
    std::vector<Epic::DAO::Meal> meals; 
    if(!Epic::DAO::Meal::find_all(meals))
    {
        ss << "Unable to load meals from db\n";
        Epic::Logging::error(ss.str());
        ss.str(std::string());
        return EXIT_FAILURE;
    }
 
    // find all foods
    std::vector<Epic::DAO::Food> foods;
    if(!Epic::DAO::Food::find_all(foods))
    {
        ss << "Unable to load foods from db\n";
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
 
    std::map< sqlite3_int64, std::vector<Epic::DAO::FoodNutrient> > nutrients_by_food_id;
    
    // find nutrients for foods
    for( std::vector<Epic::DAO::Food>::const_iterator food_it = foods.begin(), food_end = foods.end(); food_it != food_end; ++food_it)
    {
        std::vector<Epic::DAO::FoodNutrient> nutrients;
        if(food_it->find_nutrients(nutrients))
        {
            nutrients_by_food_id[food_it->get_id()] = nutrients;
        }
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
        Epic::DAO::Report::create();

        std::vector<Epic::DAO::Report> reports;
        double amount;

        std::cout << "ID,MEAL_ID,FOOD_CODE,FOOD_PORTION,NUTRIENT_CODE,NUTRIENT_QUANTITY\n" ;
        for(person_iterator_t person_it = people.begin(), person_end = people.end(); person_it != person_end; ++person_it)
        {
           // if(find_all_by_person_id(person_it->get_id(),reports))
            if(Epic::DAO::Report::find_all(person_it->get_id(),reports))
            {
                for(report_iterator_t report_it = reports.begin(), report_end = reports.end(); report_it != report_end; ++report_it)
                {
                    // reference,meal_id
                    ss << person_it->get_reference() << "," << report_it->get_meal_id() << "," ;

                    Epic::DAO::Food food = foods.at( report_it->get_food_id() -1 ); // food_id;
                    amount = report_it->get_amount() ;

                    ss << food.get_name() << "," << amount << "," ;

                    food_nutrient_map_iterator_t map_itr = nutrients_by_food_id.find(food.get_id());
                    if(map_itr != nutrients_by_food_id.end())
                    {
                        std::string line = ss.str();

                        // list scaled nutrients for food
                        for(food_nutrient_iterator_t nutrient_it = map_itr->second.begin(), nutrient_end = map_itr->second.end(); nutrient_it != nutrient_end; ++nutrient_it)
                        {
                            std::cout  << line << nutrient_it->get_nutrient_code() << "," << std::setprecision(6) << std::fixed << nutrient_it->get_scaled_amount(amount)  << "\n";
                        }
                    }

                    ss.str(std::string());
                }

                ss << "Processing completed for Respondent: " << person_it->get_reference() << "\n";
                Epic::Logging::note(ss.str());
                ss.str(std::string());
            }
        }

        // drop all the data that's been processed
        Epic::DAO::Report::destroy();
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


// find a report given a name
bool find_all_by_person_id(sqlite3_int64 person_id, std::vector<Epic::DAO::Report> & reports)
{
    Epic::Database::PreparedStatement find_all("SELECT meal_id,food_id,amount FROM person_foods_tmp WHERE person_id = ? ;");
    if(SQLITE_OK != find_all.bind_int64(1,person_id) )
    {
        std::ostringstream ss;
        ss << "Binding person_id with value: " << person_id << " failed\n" ;
        ss << "DB error was: " << Epic::Database::last_error() << "\n";
        Epic::Logging::error(ss.str());
    }

    std::vector<Epic::DAO::Report> tmp;
    tmp.swap(reports);
    for(int rc = find_all.step(); (SQLITE_ROW == rc); rc = find_all.step())
    {
        Epic::DAO::Report report;
        report.set_meal_id(find_all.column_int64(0));
        report.set_food_id(find_all.column_int64(1));
        report.set_amount(find_all.column_double(2));
        reports.push_back(report);
    }
    find_all.reset();
    return (reports.size() > 0 );
}

