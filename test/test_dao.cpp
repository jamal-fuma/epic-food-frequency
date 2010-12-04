#include "Epic_lib.hpp"

#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"
#include "dao/Nutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/Weight.hpp"
#include "dao/Frequency.hpp"
#include "dao/MealFood.hpp"
#include "dao/Portion.hpp"
#include "dao/Cereal.hpp"


void test_person();
void test_weights();
void test_nutrient();
void test_questionaire();
void test_meal();
void test_food();
void test_frequencies();
void test_questionaire_person();
void test_food_nutrient();
void test_meal_foods();
void test_portions();
void test_cereals();

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

    test_questionaire();
    test_nutrient();
    test_meal();
    test_food();
    test_weights();
    test_person();
    test_frequencies();
    test_questionaire_person();
    test_food_nutrient();
    test_meal_foods();
    test_portions();

    return EXIT_SUCCESS;
}

void test_questionaire_person()
{
    Epic::DAO::Person       person       = Epic::DAO::Person::find_by_reference("foo");
    Epic::DAO::Questionaire questionaire = Epic::DAO::Questionaire::find_by_filename("foo");

    questionaire.attach(person);

    std::vector<Epic::DAO::Person> people;
    questionaire.find_people(people);

    std::vector<Epic::DAO::Person>::const_iterator it,end;
    it = people.begin();
    end = people.end();

    std::cout << *it ;

}

void test_food_nutrient()
{
    Epic::DAO::Food food;
    Epic::DAO::Nutrient nutrient;

    nutrient.set_code(65);
    nutrient.set_description("foo bar");
    nutrient.set_units("grams");
    nutrient.save();

    food.set_name("Apple");
    food.set_description("apple treat");
    food.save();
    food.attach(nutrient,10.5);
     
    nutrient.set_code(77);
    nutrient.save();
    food.attach(nutrient,55.5);

    std::vector<Epic::DAO::FoodNutrient> nutrients;

    if(food.find_nutrients(nutrients))
    {
        std::vector<Epic::DAO::FoodNutrient>::const_iterator ci,end;
        ci   = nutrients.begin();
        end  = nutrients.end();
        for(; ci != end; ++ci)
        {
            std::cout << *ci;
        }
    }
    std::cout << "FoodNutrient [ok]" << std::endl << std::endl;
}

void test_person()
{
    Epic::DAO::Person person ;

    person.set_reference("foo");
    person.save();

    std::cout << "1st " << person;

    person.set_reference("bar");
    person.save();

    Epic::DAO::Weight weight; 
    weight.set_amount(55.0);
    weight.save();

    person.attach_visible_fat(weight);

    std::cout << "2nd " << person;

    Epic::DAO::Person foo = Epic::DAO::Person::find_by_reference("foo");

    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");

    std::cout << "Person [ok]" << std::endl << std::endl;

}

void test_questionaire()
{
    Epic::DAO::Questionaire questionaire ;

    questionaire.set_filename("foo");
    questionaire.save();

    std::cout << "1st " << questionaire;

    questionaire.set_filename("bar");
    questionaire.save();

    std::cout << "2nd " << questionaire;

    Epic::DAO::Questionaire foo =  Epic::DAO::Questionaire::find_by_filename("foo");

    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");

    std::cout << "Questionaire [ok]" << std::endl << std::endl;
}


void test_nutrient()
{
    Epic::DAO::Nutrient nutrient ;

    nutrient.set_code(100);
    nutrient.set_description("foo bar");
    nutrient.set_units("grams");
    nutrient.save();

    std::cout << "1st " << nutrient;

    nutrient.set_code(999);
    nutrient.save();

    std::cout << "2nd " << nutrient;

    Epic::DAO::Nutrient foo = Epic::DAO::Nutrient::find_by_code(100);

    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");

    std::cout << "Nutrient [ok]" << std::endl << std::endl;
}

void test_food()
{
    Epic::DAO::Food food ;

    food.set_name("foo");
    food.set_description("foo desc");
    food.save();

    std::cout << "1st " << food;

    food.set_name("bar");
    food.set_description("bar desc");
    food.save();

    std::cout << "2nd " << food;

    Epic::DAO::Food foo = Epic::DAO::Food::find_by_name("foo");

    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");

    std::cout << "Food [ok]" << std::endl << std::endl;
}

void test_meal()
{
    Epic::DAO::Meal meal ;

    meal.set_name("foo");
    meal.set_description("foo desc");
    meal.save();

    std::cout << "1st " << meal;

    meal.set_name("bar");
    meal.set_description("bar desc");
    meal.save();

    std::cout << "2nd " << meal;

    Epic::DAO::Meal foo = Epic::DAO::Meal::find_by_name("foo");

    std::cout << "3rd " << foo;

    assert(foo.valid() && "Foo is expected to be valid");

    std::cout << "Meal [ok]" << std::endl << std::endl;
}

void test_weights()
{
    Epic::DAO::Weight weight ;
    
    weight.set_amount(1.0);
    weight.save();
    std::cout << weight;
    
    weight.set_amount(0.5);
    weight.save();
    std::cout << weight;
 
    weight.set_amount(0.0);
    weight.save();
    std::cout << weight;
    
    weight.set_amount(0.0);
    weight.save();
    std::cout << weight;
    
    weight.set_amount(0.0);
    weight.save();
    std::cout << weight;

    
    Epic::DAO::Weight foo  = Epic::DAO::Weight::find_by_id(1);
    assert("weight should be valid and have amount equal to 1.0" && foo.valid() && utility_same_double(foo.get_amount(),1.0));

    foo  = Epic::DAO::Weight::find_by_id(2);
    assert("weight should be valid and have amount equal to 0.5" && foo.valid() && utility_same_double(foo.get_amount(),0.5));

    for(int x = 3; x<6; ++x)
    {
        foo  = Epic::DAO::Weight::find_by_id(x);
        assert("weight should be valid and have amount equal to 0.0" && foo.valid() && utility_same_double(foo.get_amount(),0.0));
        std::cout << foo ;
    }

    sqlite3_int64 upper,lower;
    Epic::DAO::Weight::find_bounds(upper,lower);
    assert("weight upper bounds should be == 5 " && upper == 5);
    assert("weight lower bounds should be == 1 " && lower == 1);


    std::cout << "Weight [ok]" << std::endl << std::endl;
}


void test_frequencies()
{
    Epic::DAO::Frequency frequency ;
    
    frequency.set_amount(0.0);
    frequency.save();
    std::cout << frequency;
    
    frequency.set_amount(0.07);
    frequency.save();
    std::cout << frequency;
 
    frequency.set_amount(0.14);
    frequency.save();
    std::cout << frequency;
    
    frequency.set_amount(0.43);
    frequency.save();
    std::cout << frequency;
    
    frequency.set_amount(0.79);
    frequency.save();
    std::cout << frequency;
 
    frequency.set_amount(1.00);
    frequency.save();
    std::cout << frequency;

    frequency.set_amount(2.50);
    frequency.save();
    std::cout << frequency;

    frequency.set_amount(4.50);
    frequency.save();
    std::cout << frequency;

    frequency.set_amount(6.0);
    frequency.save();
    std::cout << frequency;

    
    Epic::DAO::Frequency foo  = Epic::DAO::Frequency::find_by_id(1);
    assert("frequency should be valid and have amount equal to 0.0" && foo.valid() && utility_same_double(foo.get_amount(),0.0));

    foo  = Epic::DAO::Frequency::find_by_id(2);
    assert("frequency should be valid and have amount equal to 0.07" && foo.valid() && utility_same_double(foo.get_amount(),0.07));
 
    foo  = Epic::DAO::Frequency::find_by_id(3);
    assert("frequency should be valid and have amount equal to 0.14" && foo.valid() && utility_same_double(foo.get_amount(),0.14));

    foo  = Epic::DAO::Frequency::find_by_id(4);
    assert("frequency should be valid and have amount equal to 0.43" && foo.valid() && utility_same_double(foo.get_amount(),0.43));
 
    sqlite3_int64 upper,lower;
    
    Epic::DAO::Frequency::find_bounds(upper,lower);
    assert("frequency upper bounds should be == 9 " && upper == 9);
    assert("frequency lower bounds should be == 1 " && lower == 1);

    std::cout << "Frequency [ok]" << std::endl << std::endl;
}

void test_meal_foods()
{
    Epic::DAO::MealFood meal_food;

    meal_food.set_meal_id(Epic::DAO::Meal::find_by_name("foo").get_id());
    meal_food.set_modifier("a");
    meal_food.save();
    assert("meal_food should be invalid" && !meal_food.valid());

    std::cout << "1st " << meal_food;
    
    std::cout << "MealFood [ok]" << std::endl << std::endl;
}

void test_portions()
{
    Epic::DAO::Portion portion ;
    
    portion.set_amount(1.0);
    portion.save();
    std::cout << portion;
    
    portion.set_amount(0.5);
    portion.save();
    std::cout << portion;
}
 
void test_cereals()
{
    Epic::DAO::Cereal cereal;
    Epic::DAO::Person person;
    Epic::DAO::Food food ;
    
    person.set_reference("Fred");
    person.save();
    
    sqlite3_int64 food_ids[] = {0,0};

    food.set_name("11136");
    food.set_description("Frosties");
    food.save();
    food_ids[0] = food.get_id();
    
    food.set_name("11137");
    food.set_description("Museli");
    food.save();
    food_ids[1] = food.get_id();


    cereal.set_food_id(food_ids[0]);
    cereal.set_weight_id(1);
    cereal.save();

    person.attach_cereal(cereal);

    cereal.set_food_id(food_ids[1]);
    cereal.set_weight_id(2);
    cereal.save();

    person.attach_cereal(cereal);

    std::cout << "Cereal [ok]" << std::endl << std::endl;
}
 
