#include "../include/dataset/QuestionStatement.hpp"
#include "../include/import/Questionaire.hpp"

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
        m_qs.bind(1,*it);
        m_qs.step();
        m_qs.reset();
        question_ids_by_pos[pos] = sqlite3_last_insert_rowid( this->database() ) ;
    }

    // convert field names into the mapped equivalent objects


    // validate presence of fields
    if(pos_by_field.find("ID") == pos_by_field.end())
    {
        std::cerr << "No ID field found in questionaire, aborting processing" << std::endl;
        // will roll back automatically as wasn't commited
        return false;
    }
    return true;
}


