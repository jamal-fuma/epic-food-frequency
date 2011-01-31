/* This file is part of the libEpic client */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "dao/PersonDAO.hpp"
#include "dao/Person.hpp"
#include "dao/Weight.hpp"
#include "dao/MealFood.hpp"
#include "dao/Meal.hpp"
#include "dao/Food.hpp"
#include "dao/Portion.hpp"
#include "dao/Frequency.hpp"
#include "dao/Milk.hpp"
#include "dao/Cereal.hpp"

#include "conversion/Conversion.hpp"
#include "import/Import.hpp"


// find a person given an id
bool Epic::PersonDAO::DataAccess::find(sqlite3_int64 id, Epic::DAO::Person & person)
{
    bool rc = false;
    m_find_by_id.bind_int64(1,id);
    if(SQLITE_ROW == m_find_by_id.step())
    {
        person.set_id(id);
        person.set_reference(m_find_by_id.column_text(0));
        rc = person.validate();
    }
    m_find_by_id.reset();
    return rc;
}

// find a person given a reference
bool Epic::PersonDAO::DataAccess::find(const std::string & reference, Epic::DAO::Person & person)
{
    bool rc = false;
    m_find_by_reference.bind_text(1,reference);
    if(SQLITE_ROW == m_find_by_reference.step())
    {
        person.set_id(m_find_by_reference.column_int64(0));
        person.set_reference(reference);
        rc = person.validate();
    }
    m_find_by_reference.reset();
    return rc;
}

// save a person
bool Epic::PersonDAO::DataAccess::save(Epic::DAO::Person & person)
{
    if(person.reference_empty())
    {
        Epic::Logging::Error().log() << "Cant save a person without a reference";
        return false;
    }

    std::string fname = person.get_reference();

    // check if it exists already
    if(!find(fname,person))
    {
        m_insert.bind_text(1,fname);
        int rc = m_insert.step();
        m_insert.reset();
        if(rc == SQLITE_DONE)
        {
            person.set_id(Epic::Database::last_insert_id());
            return person.validate();
        }
    }

    // no-op as allready present in table
    return true;
}

// associate a person with a weight
bool Epic::PersonDAO::DataAccess::attach_weight(const Epic::DAO::Person & person,const Epic::DAO::Weight & weight)
{
    if(!person.valid())
    {
        Epic::Logging::Error().log() << "Cant attach a weight to an invalid person";
        return false;
    }

    if(!weight.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid weight";
        return false;
    }

    bool rc = false;
    m_attach_weight.bind_int64(1,person.get_id());
    m_attach_weight.bind_int64(2,weight.get_id());
    m_attach_weight.bind_int(3,Epic::DAO::MealFood::VisibleFat);
    rc = (SQLITE_DONE == m_attach_weight.step());
    m_attach_weight.reset();
    return rc;
}

bool Epic::PersonDAO::DataAccess::attach_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food, const Epic::DAO::MealFood::Types type)
{
    if(!person.valid())
    {
        Epic::Logging::Error().log() << "Cant attach a fat to an invalid person";
        return false;
    }

    if(!food.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid fat";
        return false;
    }

    bool rc = false;
    m_attach_fat.bind_int64(1,person.get_id());
    m_attach_fat.bind_int64(2,food.get_id());
    m_attach_fat.bind_int(3,type);
    rc = (SQLITE_DONE == m_attach_fat.step());
    m_attach_fat.reset();
    return rc;
}

bool Epic::PersonDAO::DataAccess::attach_cereal(const Epic::DAO::Person & person,const Epic::DAO::Cereal & cereal)
{
    if(!person.valid())
    {
        Epic::Logging::Error().log() << "Cant attach a cereal to an invalid person";
        return false;
    }

    if(!cereal.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid cereal";
        return false;
    }

    bool rc = false;
    m_attach_cereal.bind_int64(1,person.get_id());
    m_attach_cereal.bind_int64(2,cereal.get_food_id());
    m_attach_cereal.bind_int64(3,cereal.get_weight_id());
    m_attach_cereal.bind_int(4,Epic::DAO::MealFood::Cereal);
    rc = (SQLITE_DONE == m_attach_cereal.step());
    m_attach_cereal.reset();
    return rc;
}

// associate a person with a milk food code and portion
bool Epic::PersonDAO::DataAccess::attach_milk(const Epic::DAO::Person & person,const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion)
{
    if(!person.valid())
    {
        Epic::Logging::Error().log() << "Cant attach a milk portion to an invalid person";
        return false;
    }

    if(!milk.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid milk";
        return false;
    }

    if(!portion.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid portion of milk";
        return false;
    }

    bool rc = false;
    m_attach_food.bind_int64(1,person.get_id());
    m_attach_food.bind_int64(2,milk.get_food_id());
    m_attach_food.bind_int64(3,portion.get_id());
    rc = (SQLITE_DONE == m_attach_food.step());
    m_attach_food.reset();
    return rc;
}

// associate a person with a meal frequency
bool Epic::PersonDAO::DataAccess::attach_meal(const Epic::DAO::Person & person,const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency)
{
     if(!person.valid())
    {
        Epic::Logging::Error().log() << "Cant attach a meal frequency to an invalid person";
        return false;
    }

    if(!meal.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid meal";
        return false;
    }

    if(!frequency.valid() )
    {
        Epic::Logging::Error().log() << "Cant attach a person to an invalid frequency of meal consumption";
        return false;
    }

    bool rc = false;
    m_attach_frequency.bind_int64(1,person.get_id());
    m_attach_frequency.bind_int64(2,meal.get_id());
    m_attach_frequency.bind_int64(3,frequency.get_id());
    rc = (SQLITE_DONE == m_attach_frequency.step());
    m_attach_frequency.reset();
    return rc;

}


// associate a person with a baking fat
bool Epic::PersonDAO::DataAccess::attach_baking_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return attach_fat(person,food,Epic::DAO::MealFood::BakingFat);
}

// associate a person with a frying fat
bool Epic::PersonDAO::DataAccess::attach_frying_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return attach_fat(person,food,Epic::DAO::MealFood::FryingFat);
}


// associate a person with a meal frequency

// singleton versions

// find a person given an id
bool Epic::PersonDAO::find_by_id(sqlite3_int64 id, Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().find(id,person);
}

// find a person given a reference
bool Epic::PersonDAO::find_by_reference(const std::string & reference, Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().find(reference,person);
}

// save a person
bool Epic::PersonDAO::save(Epic::DAO::Person & person)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().save(person);
}

// associate a person with a weight
bool Epic::PersonDAO::attach_weight(const Epic::DAO::Person & person,const Epic::DAO::Weight & weight)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_weight(person,weight);
}

// associate a person with a baking_fat
bool Epic::PersonDAO::attach_baking_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_baking_fat(person,food);
}

// associate a person with a frying_fat
bool Epic::PersonDAO::attach_frying_fat(const Epic::DAO::Person & person,const Epic::DAO::Food & food)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_frying_fat(person,food);
}

// associate a person with a cereal
bool Epic::PersonDAO::attach_cereal(const Epic::DAO::Person & person,const Epic::DAO::Cereal & cereal)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_cereal(person,cereal);
}
// associate a person with a milk food code and portion
bool Epic::PersonDAO::attach_milk(const Epic::DAO::Person & person,const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_milk(person,milk,portion);
}

// associate a person with a meal frequency
bool Epic::PersonDAO::attach_meal(const Epic::DAO::Person & person,const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency)
{
    return Epic::Pattern::Singleton< Epic::PersonDAO::DataAccess >::instance().attach_meal(person,meal,frequency);
}

// wire up saving the model to the DAO
bool Epic::DAO::Person::save()
{
    return Epic::PersonDAO::save(*this);
}

// wire up associate a person with a visible fat weighting using the model
bool Epic::DAO::Person::attach_visible_fat(const Epic::DAO::Weight & weight)
{
    return Epic::PersonDAO::attach_weight(*this,weight);
}

// wire up associate a person with a baking_fat
bool Epic::DAO::Person::attach_baking_fat(const Epic::DAO::Food & food)
{
    return Epic::PersonDAO::attach_baking_fat(*this,food);
}

// wire up associate a person with a frying_fat
bool Epic::DAO::Person::attach_frying_fat(const Epic::DAO::Food & food)
{
    return Epic::PersonDAO::attach_frying_fat(*this,food);
}

// wire up associate a person with a cereal
bool Epic::DAO::Person::attach_cereal(const Epic::DAO::Cereal & cereal)
{
    return Epic::PersonDAO::attach_cereal(*this,cereal);
}

// associate a person with a milk food code and portion
bool Epic::DAO::Person::attach_milk(const Epic::DAO::Milk & milk, const Epic::DAO::Portion & portion)
{
    return Epic::PersonDAO::attach_milk(*this,milk,portion);
}

// associate a person with a meal frequency
bool Epic::DAO::Person::attach_meal(const Epic::DAO::Meal & meal, const Epic::DAO::Frequency & frequency)
{
    return Epic::PersonDAO::attach_meal(*this,meal,frequency);
}

// wire up finding the model using the DAO and a reference
Epic::DAO::Person Epic::DAO::Person::find_by_reference(const std::string & reference)
{
    Epic::DAO::Person person;
    Epic::PersonDAO::find_by_reference(reference,person);
    return person;
}

// wire up finding the model using the DAO and an id
Epic::DAO::Person Epic::DAO::Person::find_by_id(sqlite3_int64 id)
{
    Epic::DAO::Person person;
    Epic::PersonDAO::find_by_id(id,person);
    return person;
}

void
Epic::DAO::Person::process_meals(
        const Epic::Config::Config & cnf,
        const std::vector<Epic::DAO::Meal> & meals,
        const std::vector<Epic::DAO::Frequency> & frequencies,
        sqlite3_int64 frequency_upper,
        sqlite3_int64 frequency_lower)
{
    bool rc;
    std::string value;

    // find all the meal frequencies
    for( std::vector<Epic::DAO::Meal>::const_iterator ci = meals.begin(),end = meals.end(); ci != end; ++ci)
    {
        rc = cnf.find(ci->get_name(),value);
        if( (rc && value == "") || !rc)
        {
            Epic::Logging::Error().log() << "Respondent: "
                                         << this->get_reference()
                                         << " supplied "
                                         << "no frequency for meal: "
                                         << ci->get_name();
        }
        else
        {
            // need to validate frequency
            sqlite3_int64 frequency_id   = Epic::Conversion::IntegerString(value);
            if(frequency_id >= frequency_lower && frequency_id <= frequency_upper)
            {
                // find the frequency object matching this id
                this->attach_meal(*ci,frequencies.at(frequency_id -1));
            }
            else
            {
                Epic::Logging::Error().log() << "Respondent: "
                                             << this->get_reference()
                                             << " supplied "
                                             << "invalid frequency: "
                                             << value
                                             << " for meal: "
                                             << ci->get_name();
            }
        }
    }
}

// record visible fat weighting for this person
void
Epic::DAO::Person::process_visible_fat(
        const Epic::Config::Config & cnf,
        const std::vector<Epic::DAO::Weight> & weights,
        sqlite3_int64 weight_upper,
        sqlite3_int64 weight_lower)
{
    std::string value;
    bool rc = cnf.find("VISIBLE_FAT",value);
    if( (rc && value == "") || !rc)
    {
        Epic::Logging::Error().log() << "Respondent: "
                                     << this->get_reference()
                                     << " supplied "
                                     << "no visible fat weighting" ;

        Epic::Logging::Note().log() << "Respondent: "
                                     << this->get_reference()
                                     << " using default weighting: "
                                     << weight_lower ;
     }
    else
    {
        sqlite3_int64 weight_id = Epic::Conversion::IntegerString(value);
        // find the weight object matching this id
        if(weight_id >= weight_lower && weight_id <= weight_upper)
        {
            this->attach_visible_fat(weights.at(weight_id-1));
        }
        else
        {
            Epic::Logging::Error().log() << "Respondent: "
                                         << this->get_reference()
                                         << " supplied "
                                         << "invalid weight: "
                                         << value
                                         << " for visible fat";
        }
    }
}

// record cereals for this person
bool
Epic::DAO::Person::process_cereals(const Epic::Config::Config & cnf, const std::string & default_cereal)
{
    std::string value;
    std::vector<Epic::DAO::Food> foods;
    bool rc = cnf.find("CERIAL_FOOD",value);
    if(rc)
    {
        if(!this->str_to_foods(foods,value,"cereal food"))
        {
            Epic::Logging::Note().log() << "Respondent: "                 << this->get_reference()
                                        << " using default cereal code: " << default_cereal ;

            foods.push_back(Epic::DAO::Food::find_by_name( default_cereal ));

        }
        for(std::vector< Epic::DAO::Food >::const_iterator it = foods.begin(), it_end = foods.end(); it != it_end; ++it)
        {
            this->attach_cereal( Epic::DAO::Cereal::find_by_food_id(it->get_id()));
        }
    }
    return rc;
}

// record frying fats for this person
bool
Epic::DAO::Person::process_frying_fats(const Epic::Config::Config & cnf, const std::string & default_fat)
{
    std::string value;
    std::vector<Epic::DAO::Food> foods;
    bool rc = cnf.find("FAT_FRYING_FOOD",value);
    if(rc)
    {
        if(!this->str_to_foods(foods,value,"frying fat"))
        {
            Epic::Logging::Note().log() << "Respondent: " << this->get_reference()
                                        << " using default frying fat code: " << default_fat ;
            foods.push_back(Epic::DAO::Food::find_by_name( default_fat ));
        }

        for(std::vector< Epic::DAO::Food >::const_iterator it = foods.begin(), it_end = foods.end(); it != it_end; ++it)
        {
            this->attach_frying_fat(*it);
        }
    }
    return rc;
}

// record baking fats for this person
bool
Epic::DAO::Person::process_baking_fats(const Epic::Config::Config & cnf, const std::string & default_fat)
{
    std::string value;
    std::vector<Epic::DAO::Food> foods;
    bool rc = cnf.find("FAT_BAKING_FOOD",value);
    if(rc)
    {
        if(!this->str_to_foods(foods,value,"baking fat"))
        {
            Epic::Logging::Note().log() << "Respondent: " << this->get_reference()
                                        << " using default baking fat code: " << default_fat ;
            foods.push_back(Epic::DAO::Food::find_by_name( default_fat ));
        }

        for(std::vector< Epic::DAO::Food >::const_iterator it = foods.begin(), it_end = foods.end(); it != it_end; ++it)
        {
            this->attach_baking_fat(*it);
        }
    }
    return rc;
}

// record milks for this person
bool
Epic::DAO::Person::process_milk(const Epic::Config::Config & cnf,
        const std::vector<Epic::DAO::Portion> & portions,
        sqlite3_int64 portion_upper,
        sqlite3_int64 portion_lower,
        const std::string & default_milk)
{
    std::string value;

    bool rc = cnf.find("MILK_FREQUENCY",value);
    if( (rc && value == "") || !rc)
    {
         Epic::Logging::Error().log() << "Respondent: "
                                     << this->get_reference()
                                     << " supplied "
                                     << "no milk portion data" ;
        return false;
    }

    // find the weight object matching this id
    sqlite3_int64 portion_id = Epic::Conversion::IntegerString(value);
    if(!(portion_id >= portion_lower && portion_id <= portion_upper))
    {
        Epic::Logging::Error().log() << "Respondent: "
            << this->get_reference()
            << " supplied "
            << "invalid portion: "
            << value
            << " for milk";
        return false;
    }

    Epic::DAO::Portion milk_portion = portions.at(portion_id-1);

    std::vector<Epic::DAO::Food> foods;
    rc = cnf.find("MILK_FOOD",value);
    if(rc)
    {
        if(!this->str_to_foods(foods,value,"milk"))
        {
            foods.push_back(Epic::DAO::Food::find_by_name( default_milk ));
            Epic::Logging::Note().log() << "Respondent: "
                << this->get_reference()
                << " using default milk food_code: "
                << default_milk ;

        }

        for(std::vector< Epic::DAO::Food >::const_iterator it = foods.begin(), it_end = foods.end(); it != it_end; ++it)
        {
            this->attach_milk( Epic::DAO::Milk::find_by_food_id(it->get_id()), milk_portion );
        }
    }
    return rc;
}

bool
Epic::DAO::Person::str_to_foods(std::vector<Epic::DAO::Food> & foods, const std::string & value,const std::string & label)
{
    if(value == "")
    {
        Epic::Logging::Error().log() << "Respondent: " << this->get_reference() << " supplied no " << label << " food_codes" ;
        return false;
    }

    std::vector< std::string > food_codes;
    Epic::Util::Token(value).split(';').values(food_codes);

    for(std::vector< std::string >::const_iterator it = food_codes.begin(), it_end = food_codes.end(); it != it_end; ++it)
    {
        Epic::DAO::Food food = Epic::DAO::Food::find_by_name(*it);
        if(!food.valid())
        {
            Epic::Logging::Error().log() << "Respondent: "
                << this->get_reference()
                << " supplied "
                << "(" << food_codes.size()
                <<")  food_codes for "
                << label
                << " food_code ["
                << *it
                << "] wasn't found in DB]";
            continue;
        }

        foods.push_back(food);
    }
    if(foods.size() != food_codes.size())
    {
        Epic::Logging::Note().log() << "Respondent: "
            << this->get_reference()
            << " supplied "
            << "("
            << food_codes.size()
            <<")  food_codes of which ["
            << food_codes.size()-foods.size()
            << "] were invalid";
    }
    return true;
}
