#include "Epic_lib.hpp"
#include "imp.hpp"


class Person
{
    public:
        Person() {}

        // accessors
        std::string get_reference() const { 
            return m_reference ;
        }
     

        Milk get_milk() const {
            return m_milk;
        }
        

        Cereal get_primary_cereal() const {
            return m_primary_cereal;
        }

        Cereal get_secondary_cereal() const {
            return m_secondary_cereal;
        }


        BakingFat get_baking_fat() const {
            return m_bak_fat;
        }
        
        FryingFat get_frying_fat() const {
            return m_fry_fat;
        }

        // cereal mutators

        void set_primary_cereal_brand(const std::string & brand) {
            m_primary_cereal.set_brand(brand);
        }

        void set_primary_cereal_type(const std::string & type) {
            m_primary_cereal.set_type(type);
        }

        void set_secondary_cereal_brand(const std::string & brand) {
            m_secondary_cereal.set_brand(brand);
        }

        void set_secondary_cereal_type(const std::string & type) {
            m_secondary_cereal.set_type(type);
        }
       
        // baking fat mutators
        void set_baking_fat_type(const std::string & type)  { 
            m_bak_fat.set_type(type); 
        }
    
        void set_baking_fat_food(const std::string & food_code)  { 
            m_bak_fat.set_food_code(food_code); 
        }
 
        // frying fat mutators
        void set_frying_fat_type(const std::string & type)  { 
            m_fry_fat.set_type(type); 
        }
    
        void set_frying_fat_food(const std::string & food_code)  { 
            m_fry_fat.set_food_code(food_code); 
        }


        // milk mutators
        void set_milk_type(const std::string & type)  { 
            m_milk.set_type(type); 
        }
    
        void set_milk_food(const std::string & food_code)  { 
            m_milk.set_food_code(food_code); 
        }

        void set_milk_portion(const std::string & portion)  { 
            m_milk.set_portion(portion); 
        }

        // meal mutators
        void insert_meal_frequency(const std::string & meal, const std::string & frequency_str) { 
            m_meals.insert(meal,frequency_str,true);
        }

        // meat mutators
        void insert_meat_frequency(const std::string & meal, const std::string & frequency_str) { 
            m_meats.insert(meal,frequency_str,true);
        }


        // person mutators
        void set_reference(const std::string & reference) { 
            m_reference = reference; 
        }
        
        void set_eat_breakfast(const std::string & eat_breakfast) { 
            m_eat_breakfast = eat_breakfast; 
        }
        
        void set_visible_fat(const std::string & visible_fat) { 
            m_visible_fat = visible_fat; 
        }

       
    private:
        std::string             m_reference;

        std::string             m_visible_fat;
        std::string             m_eat_breakfast;
        
        Epic::Config::Config    m_meals;
        Epic::Config::Config    m_meats;
        
        Milk                    m_milk;
        
        FryingFat               m_fry_fat;
        BakingFat               m_bak_fat;

        Cereal                  m_primary_cereal;
        Cereal                  m_secondary_cereal;
};

void person_meats(Person & person, const Epic::Config::Config & cnf);
void person_meals(Person & person, const Epic::Config::Config & cnf);


int
main(int argc, char **argv)
{
    std::string conf    = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";
    std::string schema  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/sql/model.sql" ;
    std::string dbase   = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/test/foods.db" ;
    std::string import  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/test/test.csv" ;

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

    Epic::Config::Config cnf;

    load_person(cnf);

    Person person;
    std::string s;
    
    if(cnf.find("ID",s) )
    {
        person.set_reference(s);
    }
    
    person_meats(person,cnf);
    person_meals(person,cnf);

    
    // milk
    if(cnf.find("MILK",s) )
    {
        person.set_milk_type(s);
    }

    if(cnf.find("MILK_OTHER",s) )
    {
        person.set_milk_food(s);
    }

    if(cnf.find("MILK_FREQUENCY",s) )
    {
        person.set_milk_portion(s);
    }


// cereal
    if(cnf.find("CERIAL",s) )
    {
        person.insert_meal_frequency("CERIAL",s);
    }

    if(cnf.find("EAT_BREAKFAST",s) )
    {
        person.set_eat_breakfast(s);
    }

    if(cnf.find("CERIAL1",s) )
    {
        person.set_primary_cereal_type(s);
    }

    if(cnf.find("TYPE1",s) )
    {
        person.set_primary_cereal_brand(s);
    }

    if(cnf.find("CERIAL2",s) )
    {
        person.set_secondary_cereal_type(s);
    }

    if(cnf.find("TYPE2",s) )
    {
        person.set_secondary_cereal_brand(s);
    }


// frying fat
    if(cnf.find("FAT_FRYING",s) )
    {
        person.set_frying_fat_type(s);
    }

    if(cnf.find("FAT_FRYING_TYPE",s) )
    {
        person.set_frying_fat_food(s);
    }

    // frying
    if(cnf.find("CHIPS",s) )
    {
        person.insert_meal_frequency("CHIPS",s);
    }

    // frying
    if(cnf.find("ROAST_POTATOES",s) )
    {
        person.insert_meal_frequency("ROAST_POTATOES",s);
    }

    
    // baking fat
    
    if(cnf.find("FAT_BAKING",s) )
    {
        person.set_baking_fat_type(s);
    }

    if(cnf.find("FAT_BAKING_TYPE",s) )
    {
        person.set_baking_fat_food(s);
    }

    // baking
    if(cnf.find("SAVOURY_PIES",s) )
    {
        person.insert_meal_frequency("SAVOURY_PIES",s);
    }

    //baking 
    
    if(cnf.find("HOMEBAKED_CAKE",s) )
    {
        person.insert_meal_frequency("HOMEBAKED_CAKE",s);
    }

    //baking 
    
    if(cnf.find("HOMEBAKED_BUNS",s) )
    {
        person.insert_meal_frequency("HOMEBAKED_BUNS",s);
    }

    //baking 
    if(cnf.find("HOMEBAKED_FRUIT_PIES",s) )
    {
        person.insert_meal_frequency("HOMEBAKED_FRUIT_PIES",s);
    }

    // baking
    if(cnf.find("HOMEBAKED_SPONGE",s) )
    {
        person.insert_meal_frequency("HOMEBAKED_SPONGE",s);
    }


    // baking
    if(cnf.find("SAUCES",s) )
    {
        person.insert_meal_frequency("SAUCES",s);
    }

 
    Milk milk = person.get_milk();

    std::cout << "Milk : type (" << milk.get_type() << ") food_code (" << milk.get_food_code() << ") portion (" << milk.get_portion() << ")" << std::endl;

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
  
    std::string s;

    // load all meats
    for(; it != end; ++it)
    {
        if(cnf.find(*it,s) )
        {
            person.insert_meat_frequency(*it,s);
        }
    }
    
    // visible fat
    if(cnf.find("VISIBLE_FAT",s) )
    {
        person.set_visible_fat(s);
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
