#include "Epic_lib.hpp"
#include "imp.hpp"

#include "dao/Meal.hpp"
#include "dao/Frequency.hpp"
#include "dao/Weight.hpp"

#include "import/Import.hpp"
#include "conversion/Conversion.hpp"

struct response
{
    size_t        line;
    sqlite3_int64 meal_id;
    sqlite3_int64 frequency_id;
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
    Epic::Import::str_vector_t v,h;

    Epic::Config::Config cnf;

    
    std::vector<response> responses;
    sqlite3_int64 frequency_upper,frequency_lower;
    sqlite3_int64 weight_upper,weight_lower;
    
    Epic::DAO::Frequency::find_bounds(frequency_upper,frequency_lower);
    Epic::DAO::Weight::find_bounds(weight_upper,weight_lower);

    std::ostringstream ss;
    
    std::map<sqlite3_int64,Epic::DAO::Frequency> frequency_by_id;
    std::map<sqlite3_int64,Epic::DAO::Weight>    weight_by_id;
    
    // find all meals
    std::vector<Epic::DAO::Meal> meals; 
    if(!Epic::DAO::Meal::find_all(meals))
    {
        ss << "Unable to load meals from db\n";
        Epic::Logging::error(ss.str());
        ss.str("");
        return EXIT_FAILURE;
    }
            
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

            Person person;
            std::string value;

            if(cnf.find("ID",value) )
            {
                person.set_reference(value);
            }
            
            // find all the meal frequencies
            std::vector<Epic::DAO::Meal>::const_iterator ci,end;
            end = meals.end();
            for(ci  = meals.begin(); ci != end; ++ci)
            {
                response resp;
                if(cnf.find(ci->get_name(),value))
                {
                    resp.line           = line;
                    resp.meal_id        = ci->get_id();

                    // need to validate frequency 
                    resp.frequency_id   = Epic::Conversion::IntegerString(value);
                    if(resp.frequency_id >= frequency_lower && resp.frequency_id <= frequency_upper)
                        responses.push_back(resp);
                    else
                    {
                        ss << "Respondent: " << person.get_reference() << " supplied ";
                        ss << "invalid frequency:  " << value << " for meal: " << ci->get_name() << std::endl; 
                        Epic::Logging::error(ss.str());
                        ss.str("");
                    }
                }
            }

            person_meats(person,cnf);
            person_meals(person,cnf);
            person_milks(person,cnf);
            person_cereals(person,cnf);
            person_frying_fats(person,cnf);
            person_baking_fats(person,cnf);

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
            std::cout << "Cereal 1    : brand (" << cereal_1.get_brand() << ") " << "type (" << cereal_1.get_type() << ")" << std::endl;
            std::cout << "Cereal 2    : brand (" << cereal_2.get_brand() << ") " << "type (" << cereal_2.get_type() << ")" << std::endl;
            std::cout << "Cereals     : food_codes (" << person.get_cereal_foods() << ")" << std::endl;
            std::cout << "BakingFat   : type (" << bfat.get_type() << ") other (" << bfat.get_food_code() << ") food_code (" << person.get_frying_fat_foods() << ")" << std::endl;
            std::cout << "FryingFat   : type (" << ffat.get_type() << ") other (" << ffat.get_food_code() << ") food_code (" << person.get_baking_fat_foods() << ")" << std::endl;
            std::cout << std::endl <<std::endl;
        }
    }

    return EXIT_SUCCESS;
}




void person_meats(Person & person, const Epic::Config::Config & cnf)
{
   // meat - meals with visible fat component
    std::vector< std::string > meats;
    load_meat_names(meats);

    std::vector< std::string >::const_iterator it,end;
    it  = meats.begin();
    end = meats.end();

    std::string value;

    // load all meats
    for(; it != end; ++it)
    {
        if(cnf.find(*it,value) )
        {
            person.insert_meat_frequency(*it,value);
        }
    }

    // visible fat
    if(cnf.find("VISIBLE_FAT",value) )
    {
        person.set_visible_fat(value);
    }
}


void person_meals(Person & person, const Epic::Config::Config & cnf)
{

    // simple foods without any funny processing
    std::vector< std::string > simples;
    load_simple_names(simples);


    std::vector< std::string >::const_iterator it,end;
    it  = simples.begin();
    end = simples.end();

    std::string value;

    // load all simple foods
    for(; it != end; ++it)
    {
        if(cnf.find(*it,value) )
        {
            person.insert_meal_frequency(*it,value);
        }
    }
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
}

void person_cereals(Person & person, const Epic::Config::Config & cnf)
{
    std::string value;

// cereal
    if(cnf.find("CERIAL",value) )
    {
        person.insert_meal_frequency("CERIAL",value);
    }

    if(cnf.find("CERIAL_FOOD",value) )
    {
        person.set_cereal_foods(value);
    }


    if(cnf.find("EAT_BREAKFAST",value) )
    {
        person.set_eat_breakfast(value);
    }

    if(cnf.find("CERIAL1",value) )
    {
        person.set_primary_cereal_brand(value);
    }

    if(cnf.find("TYPE1",value) )
    {
        person.set_primary_cereal_type(value);
    }

    if(cnf.find("CERIAL2",value) )
    {
        person.set_secondary_cereal_brand(value);
    }

    if(cnf.find("TYPE2",value) )
    {
        person.set_secondary_cereal_type(value);
    }
}

void person_frying_fats(Person & person, const Epic::Config::Config & cnf)
{
    std::string value;

    // frying fat
    if(cnf.find("FAT_FRYING",value) )
    {
        person.set_frying_fat_type(value);
    }

    if(cnf.find("FAT_FRYING_TYPE",value) )
    {
        person.set_frying_fat_food(value);
    }

    if(cnf.find("FAT_FRYING_FOOD",value) )
    {
        person.set_fry_fat_foods(value);
    }


    // frying
    if(cnf.find("CHIPS",value) )
    {
        person.insert_frying_fat_frequency("CHIPS",value);
    }

    // frying
    if(cnf.find("ROAST_POTATOES",value) )
    {
        person.insert_frying_fat_frequency("ROAST_POTATOES",value);
    }
}

void person_baking_fats(Person & person, const Epic::Config::Config & cnf)
{
    std::string value;

    // baking fat

    if(cnf.find("FAT_BAKING",value) )
    {
        person.set_baking_fat_type(value);
    }

    if(cnf.find("FAT_BAKING_TYPE",value) )
    {
        person.set_baking_fat_food(value);
    }

    if(cnf.find("FAT_BAKING_FOOD",value) )
    {
        person.set_bak_fat_foods(value);
    }


    // baking
    if(cnf.find("SAVOURY_PIES",value) )
    {
        person.insert_meal_frequency("SAVOURY_PIES",value);
    }

    //baking

    if(cnf.find("HOMEBAKED_CAKE",value) )
    {
        person.insert_meal_frequency("HOMEBAKED_CAKE",value);
    }

    //baking

    if(cnf.find("HOMEBAKED_BUNS",value) )
    {
        person.insert_meal_frequency("HOMEBAKED_BUNS",value);
    }

    //baking
    if(cnf.find("HOMEBAKED_FRUIT_PIES",value) )
    {
        person.insert_meal_frequency("HOMEBAKED_FRUIT_PIES",value);
    }

    // baking
    if(cnf.find("HOMEBAKED_SPONGE",value) )
    {
        person.insert_meal_frequency("HOMEBAKED_SPONGE",value);
    }


    // baking
    if(cnf.find("SAUCES",value) )
    {
        person.insert_meal_frequency("SAUCES",value);
    }
}
