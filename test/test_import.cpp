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

        // frying fat mutators
        void insert_frying_fat_frequency(const std::string & meal, const std::string & frequency_str) {
            m_ffats.insert(meal,frequency_str,true);
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
        Epic::Config::Config    m_ffats;

        Milk                    m_milk;

        FryingFat               m_fry_fat;
        BakingFat               m_bak_fat;

        Cereal                  m_primary_cereal;
        Cereal                  m_secondary_cereal;
};

void person_meats(Person & person, const Epic::Config::Config & cnf);
void person_meals(Person & person, const Epic::Config::Config & cnf);
void person_milks(Person & person, const Epic::Config::Config & cnf);
void person_cereals(Person & person, const Epic::Config::Config & cnf);
void person_frying_fats(Person & person, const Epic::Config::Config & cnf);
void person_baking_fats(Person & person, const Epic::Config::Config & cnf);


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
    std::string value;

    if(cnf.find("ID",value) )
    {
        person.set_reference(value);
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

    std::cout << "Milk      : type (" << milk.get_type() << ") food_code (" << milk.get_food_code() << ") portion (" << milk.get_portion() << ")" << std::endl;
    std::cout << "Cereal 1  : type (" << cereal_1.get_type() << ") brand (" << cereal_1.get_brand() << ")" << std::endl;
    std::cout << "Cereal 2  : type (" << cereal_2.get_type() << ") brand (" << cereal_2.get_brand() << ")" << std::endl;
    std::cout << "BakingFat : type (" << bfat.get_type() << ") food_code (" << bfat.get_food_code() << ")" << std::endl;
    std::cout << "FryingFat : type (" << ffat.get_type() << ") food_code (" << ffat.get_food_code() << ")" << std::endl;

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

    if(cnf.find("MILK_OTHER",value) )
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

    if(cnf.find("EAT_BREAKFAST",value) )
    {
        person.set_eat_breakfast(value);
    }

    if(cnf.find("CERIAL1",value) )
    {
        person.set_primary_cereal_type(value);
    }

    if(cnf.find("TYPE1",value) )
    {
        person.set_primary_cereal_brand(value);
    }

    if(cnf.find("CERIAL2",value) )
    {
        person.set_secondary_cereal_type(value);
    }

    if(cnf.find("TYPE2",value) )
    {
        person.set_secondary_cereal_brand(value);
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
