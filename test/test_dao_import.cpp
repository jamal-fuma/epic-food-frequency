#include "Epic_lib.hpp"
#include "imp.hpp"

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
            std::vector<Epic::DAO::Meal>::const_iterator ci,end;
            end = meals.end();
            for(ci  = meals.begin(); ci != end; ++ci)
            {
                response resp;
                if(cnf.find(ci->get_name(),value))
                {
                    resp.person_id      = person.get_id();
                    resp.meal_id        = ci->get_id();

                    // need to validate frequency 
                    resp.frequency_id   = Epic::Conversion::IntegerString(value);
                    if(resp.frequency_id >= frequency_lower && resp.frequency_id <= frequency_upper)
                    {
                        // find the frequency object matching this id
                        person.attach_meal(*ci,Epic::DAO::Frequency::find_by_id(resp.frequency_id));
                        // responses.push_back(resp);
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
                if(weight_id >= weight_lower && weight_id <= weight_upper)
                {
                    // find the weight object matching this id
                    person.attach_visible_fat(Epic::DAO::Weight::find_by_id(weight_id));
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
 
    // create a temporary table to speed up processing
    Epic::Database::execute("BEGIN; CREATE TEMPORARY TABLE person_foods_tmp AS SELECT person_id,meal_id,food_id,amount FROM person_food_vw ; CREATE INDEX index_person_foods_tmp_on_person_meal_food ON person_foods_tmp(person_id,meal_id,food_id); COMMIT;");
    std::vector<Epic::DAO::Person> people;
    if(questionaire.find_people(people))
    {
        Epic::Database::PreparedStatement sql("SELECT meal_id,food_id,amount FROM person_foods_tmp WHERE person_id=? ;");

        for(std::vector<Epic::DAO::Person>::const_iterator it = people.begin(), end = people.end(); it != end; ++it)
        {
            sql.bind_int64(1,it->get_id());

            for(int rc = sql.step(); (SQLITE_ROW == rc); rc = sql.step())
            {
                ss << it->get_reference() << ",";      // reference 
                ss << sql.column_int64(0) << "," ;     // meal_id
                
                double amount = sql.column_double(2);  // food_amount
                
                Epic::DAO::Food food = foods.at(sql.column_int64(1) -1 ); // food_id;
                
                // skip foods with zero consumption
//              Epic::DAO::Food food = Epic::DAO::Food::find_by_id(sql.column_int64(1) ); // food_id;

                ss << food.get_name() << "," << amount << "," ;

                std::map< sqlite3_int64, std::vector<Epic::DAO::FoodNutrient> >::const_iterator map_itr;
                
                if((map_itr = nutrients_by_food_id.find(food.get_id())) != nutrients_by_food_id.end())
                {
                    std::string line = ss.str();

                    // list scaled nutrients for food
                    for(std::vector<Epic::DAO::FoodNutrient>::const_iterator nutrient_it = map_itr->second.begin(), nutrient_end = map_itr->second.end(); nutrient_it != nutrient_end; ++nutrient_it)
                    {
                        std::cout  << line << nutrient_it->get_nutrient_code() << "," << nutrient_it->get_scaled_amount(amount) << "," << "\n";
                    }
                }

                ss.str(std::string());
            }
            sql.reset();

            ss << "Processing completed for Respondent: " << it->get_reference() << "\n";
            Epic::Logging::note(ss.str());
            ss.str(std::string());
        }
            
        // drop all the data that's been processed
        ss << "BEGIN; DELETE FROM questionaires where id =" <<questionaire.get_id() << "; COMMIT;" ;
        Epic::Database::execute(ss.str());
        ss.str(std::string());
    }

    Epic::Database::execute("BEGIN; DROP INDEX index_person_foods_tmp_on_person_meal_food; DROP TABLE person_foods_tmp; COMMIT; VACUUM;");

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
