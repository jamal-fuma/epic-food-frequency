#include "client/Application.hpp"
#include "import/Nutrient.hpp"
#include "import/Food.hpp"
#include "import/Meal.hpp"
#include "import/QuestionTypes.hpp"
#include "import/Frequency.hpp"
#include "import/Group.hpp"
#include "import/Weight.hpp"
#include "import/Ingredient.hpp"
#include "dataset/Report.hpp"


template <class Data>
bool
Epic::Client::Application::load_lookup(Database::DBConnection & db,const std::string & config_key)
{
    std::string value;
    std::ostringstream ss;

    if(!configured_value(config_key,value))
    {
        ss << "Config file lacks value for '" ;
        ss << config_key;
        ss << "'\n" ;
        m_logger.error(ss.str());
        return false;
    }

    Data importer(db);
    if(!Import::load(value,importer))
    {
        ss << "Loading imports for '" ;
        ss << config_key;
        ss << "' failed\n" ;
        m_logger.error(ss.str());
        return false;
    }

    ss << "Loading imports for '" ;
    ss << config_key;
    ss << "' completed\n" ;
    m_logger.note(ss.str());
    return importer.commit();
}

// need to populate the lookup tables
bool
Epic::Client::Application::load_tables(Database::DBConnection & db)
{
    if(!load_lookup< Import::NutrientData >(db,"nutrients"))
        return false;

    if(!load_lookup< Import::FoodData >(db,"foods"))
        return false;

    if(!load_lookup< Import::MealData >(db,"meals"))
        return false;
    
    if(!load_lookup< Import::QuestionTypeData >(db,"question_types"))
        return false;
    
    if(!load_lookup< Import::FrequencyData >(db,"frequencies"))
        return false;
    
    if(!load_lookup< Import::GroupData >(db,"groups"))
        return false;
    
    if(!load_lookup< Import::WeightData >(db,"weights"))
        return false;
 
    if(!load_lookup< Import::IngredientData >(db,"ingredients"))
        return false;

    return true;
}

int
Epic::Client::Application::run()
{
    try
    {
        // need a database to work on
        std::string db_name;
        if(!configured_value("database",db_name))
        {
            m_logger.error("Config file lacks value for 'database'");
            return EXIT_FAILURE;
        }

        Database::DBConnection db(db_name);

        if(!create_tables(db))
        {
            m_logger.error("Creating db tables failed\n");
            return EXIT_FAILURE;
        }

        if(!load_tables(db))
        {
            m_logger.error("Populating db tables failed\n");
            return EXIT_FAILURE;
         }

        // parse data into the database
        if(!load_questionaire(db))
        {
            m_logger.error("Questionaire processing not started due to insuffient input\n");
            m_logger.note("Questionaire may be specifed as single file input or in a batch using a jobfile\n");
            help();   
            return EXIT_FAILURE;
        }

        // need to wire output filename here
        Database::Report rs (db);
        rs.list_all(std::cout);
    }
    catch(std::runtime_error & e)
    {
        m_logger.error(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

bool
Epic::Client::Application::create_tables(Database::DBConnection & db)
{
    static const char schema[] = { EPIC_DB_SCHEMA };
    char *errmsg=NULL;

    /* create db tables */
    if(SQLITE_OK != sqlite3_exec(db,
                schema,
                NULL,
                NULL,
                &errmsg))
    {
        std::ostringstream ss;

        ss << "Could not execute statement in database ";
        ss << schema << " library error was " << errmsg << std::endl;

        /* sqlite uses it's own  pool so need to clean up it's errors */
        sqlite3_free(errmsg);
        errmsg = NULL;

        m_logger.error(ss.str());
        return false;
    }
    return true;
}
