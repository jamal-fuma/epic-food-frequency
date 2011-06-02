#include <stdlib.h>
#include <assert.h>


#include "config/Global.hpp"
#include "libdao/Database.hpp"

#include "dao/Person.hpp"
#include "dao/Food.hpp"
#include "dao/Cereal.hpp"


void test_cereals();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();
    test_cereals();
    return EXIT_SUCCESS;
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
}
