#include "Epic_lib.hpp"

#include "dao/Questionaire.hpp"
#include "dao/Person.hpp"
#include "dao/Nutrient.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/Weight.hpp"


void test_person();
void test_weights();
void test_nutrient();
void test_questionaire();
void test_meal();
void test_food();
void test_questionaire_person();

/* all food_codes for meats with visible fat portion scaled correctly 
select meal_id,food,(amount+0.0) from ingredients where meal_id=1 and modifier="" 
union select meal_id, food,(ingredients.amount * weights.amount) from ingredients,weights where weights.code=1 and ingredients.modifier="a" and ingredients.meal_id =1 
*/


/* MEAT meal names
select meals.name from ingredients inner join meals on ingredients.meal_id = meals.id  where ingredients.modifier = "a";
*/

/* BAKING fat meal names
select meals.name from ingredients inner join meals on ingredients.meal_id = meals.id  where ingredients.modifier = "b";
*/

/* CERIAL meal names
select meals.name, meals.id from ingredients inner join meals on ingredients.meal_id = meals.id  where ingredients.modifier = "c";
*/

/* FRYING fat meal names
select meals.name, meals.id from ingredients inner join meals on ingredients.meal_id = meals.id  where ingredients.modifier = "d";
*/

/* Simple foods
  select meals.name,meals.id from meals where meals.id in (select meals.id from meals except select ingredients.meal_id from ingredients where ingredients.modifier in ("a","b","c","d") )  ;
*/

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

    test_person();
    test_questionaire();
    test_nutrient();
    test_meal();
    test_food();
    test_weights();
    test_questionaire_person();
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

void test_person()
{
    Epic::DAO::Person person ;

    person.set_reference("foo");
    person.save();

    std::cout << "1st " << person;

    person.set_reference("bar");
    person.save();

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
    std::cout << "Weight [ok]" << std::endl << std::endl;
}
