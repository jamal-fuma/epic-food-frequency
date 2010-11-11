#include "../include/dataset/QuestionStatement.hpp"
#include "../include/import/Questionaire.hpp"

struct Milk
{
   std::string type;
   std::string other;
   std::string portion;
};

struct Person
{
    Milk milk;
};

bool
Epic::Import::QuestionaireDataModel::header(str_vector_t & v)
{
  // header
    str_vector_t::iterator it,end;
    it = v.begin(); end = v.end();

    for(size_t pos=0; it != end; ++pos,++it)
    {
        fields_by_pos[pos] = *it;
        pos_by_field[*it] = pos;


		// group is being assigned here
        m_qs.bind(1,*it);
	m_qs.step();
        m_qs.reset();


        question_ids_by_pos[pos] = Epic::Database::last_insert_id() ;
    }


    Person person;
    
    Milk milk;
    
    if(pos_by_field.find("MILK") != pos_by_field.end())
        milk.type = v.at(pos_by_field["MILK"]);

    if(pos_by_field.find("MILK_OTHER") != pos_by_field.end())
        milk.other = v.at(pos_by_field["MILK_OTHER"]);
 
    if(pos_by_field.find("MILK_FREQUENCY") != pos_by_field.end())
        milk.portion = v.at(pos_by_field["MILK_FREQUENCY"]);


    std::string group; // how derived ??

    // convert field names into the mapped equivalent objects

    if(group == "Cereal")
    {
        // type
        // brand
    }

    else if(group == "Milk")
    {
        // type
        // other
        // frequency is really a portion 
        // multiplied by frequency code for daily usage
    }

    else if(group == "BakingFat")
    {
        // type
        // other
    }

    else if(group == "FryingFat")
    {
        // type
        // other
    }

    else if(group == "Frequency")
    {
        // value lookup in meals table
        // select ingredients.food,
        //        ingredients.amount 
        //  from meals,
        //       ingredients 
        //  where
        //      ingredients.meal_id = meals.line 
        //  and meal.name = ?;
    }

    else if(group == "ID" )
    {
    }

    else if(group == "VisibleFat")
    {
        // type - weighted additional food code
    }

    else if(group == "Discard")
    {
    }

    // validate presence of fields
    if(pos_by_field.find("ID") == pos_by_field.end())
    {
        std::cerr << "No ID field found in questionaire, aborting processing" << std::endl;
        // will roll back automatically as wasn't commited
        return false;
    }
    return true;
}


