#include "../include/import/Import.hpp"
#include "../include/dataset/Database.hpp"

namespace Epic
{
    namespace Import
    {
        struct DataModel : public DBModel
        {
            DataModel(Database::DBConnection & db) :
                DBModel(db)
            {
            }

            bool header(str_vector_t & names);
            bool body(size_t line, str_vector_t & values);
        };

        struct ModelData : ImportModel<DataModel>
        {
            ModelData(Database::DBConnection & db) :
                 ImportModel<DataModel>(db) {}
        };

    } // Epic::Import

    namespace Detail
    {
        struct Questionaire {
            sqlite3_int64 m_id;
            std::string m_filename;

            Questionaire() {}
            Questionaire(sqlite3_int64 id, const std::string & filename) :
                m_id(id), m_filename(filename) {}
        };

        struct Person {
            sqlite3_int64 m_id;
            std::string m_reference;

            Person(){}
            Person(sqlite3_int64 id, const std::string & reference) :
                m_id(id), m_reference(reference) {}
        };

        struct QuestionairePerson {
            sqlite3_int64 m_questionaire_id;
            sqlite3_int64 m_person_id;

            QuestionairePerson(){}
            QuestionairePerson(sqlite3_int64 questionaire_id, sqlite3_int64 person_id) :
                m_questionaire_id(questionaire_id),  m_person_id(person_id) {}
        };

        struct PersonMeal {
            sqlite3_int64 m_person_id;
            sqlite3_int64 m_meal_id;
            double m_frequency;

            PersonMeal(){}
            PersonMeal(sqlite3_int64 person_id, sqlite3_int64 meal_id, double frequency) :
                m_person_id(person_id),  m_meal_id(meal_id), m_frequency(frequency) {}
        };

        struct PersonFood {
            sqlite3_int64 m_person_id;
            sqlite3_int64 m_food_id;
            sqlite3_int64 m_portion;
            double m_frequency;

            PersonFood(){}
            PersonFood(sqlite3_int64 person_id, sqlite3_int64 food_id, sqlite3_int64 portion, double frequency) :
                m_person_id(person_id),  m_food_id(food_id),  m_portion(portion), m_frequency(frequency) {}
        };

        struct Meal {
            sqlite3_int64 m_id;
            std::string m_name;
            std::string m_description;

            Meal() {}
            Meal(sqlite3_int64 id, const std::string & name, const std::string & description) :
                m_id(id), m_name(name), m_description(description) {}
        };

        struct Food {
            sqlite3_int64 m_id;
            std::string m_name;
            std::string m_description;

            Food(){}
            Food(sqlite3_int64 id, const std::string & name, const std::string & description) :
                m_id(id), m_name(name), m_description(description) {}
        };

        struct Nutrient {
            sqlite3_int64 m_id;
            sqlite3_int64 m_code;
            std::string m_description;
            std::string m_units;

            Nutrient() {}
            Nutrient(sqlite3_int64 id, const std::string & name, sqlite3_int64 code, const std::string & description, const std::string & units) :
                m_id(id), m_code(code), m_description(description), m_units(units) {}

        };

        struct MealFood {
            sqlite3_int64 m_meal_id;
            sqlite3_int64 m_food_id;
            sqlite3_int64 m_portion;

            MealFood(){}
            MealFood(sqlite3_int64 meal_id, sqlite3_int64 food_id, sqlite3_int64 portion) :
                m_meal_id(meal_id),  m_food_id(food_id),  m_portion(portion) {}
        };

        struct FoodNutrient {
            sqlite3_int64 m_food_id;
            sqlite3_int64 m_nutrient_id;
            double m_quantity;

            FoodNutrient(){}
            FoodNutrient(sqlite3_int64 food_id, sqlite3_int64 nutrient_id, double quantity) :
                m_food_id(food_id),  m_nutrient_id(nutrient_id), m_quantity(quantity) {}
        };
    }
}
