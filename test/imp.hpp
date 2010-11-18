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

#endif
