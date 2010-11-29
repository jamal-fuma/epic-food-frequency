#include "Epic_lib.hpp"

#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"
#include "dao/Nutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/MealFood.hpp"
#include "dao/Weight.hpp"
#include "dao/Frequency.hpp"

#include "import/Import.hpp"

bool load_foods(const std::string & filename);
bool load_nutrients(const std::string & filename);
bool load_meals(const std::string & filename);
bool load_food_nutrients(const std::string & filename);
bool load_meal_foods(const std::string & filename);
bool load_weights(const std::string & filename);
bool load_frequencies(const std::string & filename);
void test_imports();


int
main(int argc, char **argv)
{
    std::string conf    = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";
    std::string schema  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/sql/model.sql" ;
    std::string dbase   = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/test/foods.db" ;
    
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

    test_imports();

    // find all meals with visible fat
    std::vector<Epic::DAO::Meal> meals; 
    if(Epic::DAO::Meal::find_all_with_visible_fat(meals))
    {
        std::vector<Epic::DAO::Meal>::const_iterator ci,end;
        end = meals.end();
        std::vector<Epic::DAO::MealFood> foods; 
        for(ci  = meals.begin(); ci != end; ++ci)
        {
            foods.clear();
            std::cout << *ci;
            if(ci->find_all_foods(foods))
            {
                std::vector<Epic::DAO::MealFood>::const_iterator food_it, food_end;
                food_end = foods.end();
                for(food_it  = foods.begin(); food_it != food_end; ++food_it)
                {
                    std::cout << "\t " <<  *food_it;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

void test_imports()
{
    std::string foods_file  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/import/model/foods.csv" ;
    std::string food_nutrients_file  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/import/model/food_nutrients.csv" ;
    std::string meal_foods_file  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/import/model/meal_foods.csv" ;


    std::string s;
    load_foods(foods_file);
    if( Epic::Config::find("nutrients",s) )
    {
        load_nutrients(s);
    }
    else
    {
        std::cerr << "nutrients not present in config" << std::endl;
    }

    load_food_nutrients(food_nutrients_file);

    if( Epic::Config::find("meals",s) )
    {
        load_meals(s);
    }
    else
    {
        std::cerr << "meals not present in config" << std::endl;
    }
    if( Epic::Config::find("weights",s) )
    {
        load_weights(s);
    }
    else
    {
        std::cerr << "weights not present in config" << std::endl;
    }
    if( Epic::Config::find("frequencies",s) )
    {
        load_frequencies(s);
    }
    else
    {
        std::cerr << "frequencies not present in config" << std::endl;
    }

    load_meal_foods(meal_foods_file);
}



bool load_foods(const std::string & filename)
{
    return Epic::DAO::Food::load(filename);
}

bool load_meals(const std::string & filename)
{
    return Epic::DAO::Meal::load(filename);
}

bool load_nutrients(const std::string & filename)
{
    return Epic::DAO::Nutrient::load(filename);
}

bool load_food_nutrients(const std::string & filename)
{
    return Epic::DAO::FoodNutrient::load(filename);
}

bool load_meal_foods(const std::string & filename)
{
    return Epic::DAO::MealFood::load(filename);
}

bool load_weights(const std::string & filename)
{
    return Epic::DAO::Weight::load(filename);
}

bool load_frequencies(const std::string & filename)
{
    return Epic::DAO::Frequency::load(filename);
}
