#include "../include/import/Questionaire.hpp"

bool
Epic::Import::QuestionaireDataModel::body(size_t line, str_vector_t & v)
{
    // new line, means we have a respondant ID
    m_respondent.bind(v.at(pos_by_field["ID"]));
    m_respondent.step();
    m_respondent.reset();

    sqlite3_int64 respondent_id = Epic::Database::last_insert_id();

    // insert {respondent, field name, field value} triplets
    str_vector_t::size_type end = v.size();
    for(str_vector_t::size_type pos=0; pos != end; ++pos)
    {
        // this is unsafe question_ids_by_pos[pos] might not be correct
        m_response.bind(respondent_id,question_ids_by_pos[pos],v.at(pos));
        m_response.step();
        m_response.reset();
    }

    // questionaire processing
    //
    // frequencies
    //
    // free text
    //
    //
    return true;
}

