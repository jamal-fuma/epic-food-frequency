#ifndef IMP_HPP
#define IMP_HPP

#include <string>
#include <vector>
#include "Epic_lib.hpp"

class Food
{
public:
    Food() {}

    // accessors
    std::string get_food_code() const {
        return m_food_code ;
    }

    // mutators
    void set_food_code(const std::string & food_code) {
        m_food_code = food_code;
    }

private:
    std::string m_food_code;
};

class Cereal
{
public:
    Cereal() {}

    // accessors
    std::string get_type() const {
        return m_type.get_food_code() ;
    }

    std::string get_brand() const {
        return m_brand.get_food_code() ;
    }


    // mutators

    void set_type(const std::string & food_code) {
        m_type.set_food_code(food_code);
    }

    void set_brand(const std::string & food_code) {
        m_brand.set_food_code(food_code);
    }

private:
    Food m_type;
    Food m_brand;
};

class FoodType : public Food
{
public:
    FoodType() {}

    // accessors
    std::string get_type() const {
        return m_type ;
    }

    // mutators
    void set_type(const std::string & type) {
        m_type = type;
    }

private:
    std::string m_type;
};

class FryingFat  : public FoodType
{
};

class BakingFat  : public FoodType
{
};

class Milk : public FoodType
{
    public:
        Milk() {}

    // accessors
    std::string get_portion() const {
        return m_portion ;
    }

    // mutators
    void set_portion(const std::string & portion) {
        if(portion == "1")
            m_portion = "0";

        else if(portion == "2")
            m_portion = "146";

        else if(portion == "3")
            m_portion = "293";

        else if(portion == "4")
            m_portion = "439";

        else if(portion == "5")
            m_portion = "585";

        else if(portion == "6")
            m_portion = "878";
        else
            m_portion = portion;
    }
private:
    std::string m_portion;
};


void load_header(std::vector< std::string > & header);
void load_row(std::vector< std::string > & row);
void load_person(Epic::Config::Config & cnf);
void load_simple_names(std::vector<std::string> & simples);
void load_meat_names(std::vector<std::string> & meats);

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

        std::string get_frying_fat_foods(){
            return m_fry_fat_foods;
        }

        std::string get_visible_fat() {
            return m_visible_fat;
        }

        std::string get_baking_fat_foods(){
            return m_bak_fat_foods;
        }

        bool get_meal_frequency(const std::string & meal, std::string & frequency_str) {
            return m_meals.find(meal,frequency_str);
        }

        std::string get_cereal_foods(){
            return m_cereal_foods;
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

        void set_bak_fat_foods(const std::string & bak_fat_foods) {
            m_bak_fat_foods = bak_fat_foods;
        }


        // frying fat mutators
        void set_frying_fat_type(const std::string & type)  {
            m_fry_fat.set_type(type);
        }

        void set_frying_fat_food(const std::string & food_code)  {
            m_fry_fat.set_food_code(food_code);
        }

        void set_fry_fat_foods(const std::string & fry_fat_foods) {
            m_fry_fat_foods = fry_fat_foods;
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

        void set_cereal_foods(const std::string & cereal_foods) {
            m_cereal_foods = cereal_foods;
        }


    private:
        std::string             m_reference;

        std::string             m_visible_fat;

        std::string             m_cereal_foods;
        std::string             m_bak_fat_foods;
        std::string             m_fry_fat_foods;

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


#endif
