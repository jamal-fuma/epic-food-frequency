#include "client/Application.hpp"
#include "import/Nutrient.hpp"
#include "import/Food.hpp"
#include "import/Meal.hpp"
#include "import/QuestionTypes.hpp"
#include "import/Frequency.hpp"
#include "import/Group.hpp"
#include "import/Weight.hpp"
#include "import/Ingredient.hpp"
#include "import/CerealTypes.hpp"
#include "import/MilkTypes.hpp"
#include "dataset/Report.hpp"


template <class Data>
bool
Epic::Client::Application::load_lookup(const std::string & config_key)
{
    std::string value;
    std::ostringstream ss;

    if(!Epic::Config::find(config_key,value))
    {
        ss << "Config file lacks value for '" << config_key << "'\n" ;
        Epic::Logging::error(ss.str());
        return false;
    }

    Data importer;
    if(!Import::load(value,importer))
    {
        ss << "Loading imports for '" << config_key << "' failed\n" ;
        Epic::Logging::error(ss.str());
        return false;
    }

    ss << "Loading imports for '" << config_key << "' completed\n" ;
    Epic::Logging::note(ss.str());
    return importer.commit();
}

// need to populate the lookup tables
bool
Epic::Client::Application::load_tables()
{
    if(!load_lookup< Import::NutrientData >("nutrients"))
        return false;

    if(!load_lookup< Import::FoodData >("foods"))
        return false;

    if(!load_lookup< Import::MealData >("meals"))
        return false;

    if(!load_lookup< Import::QuestionTypeData >("question_types"))
        return false;

    if(!load_lookup< Import::FrequencyData >("frequencies"))
        return false;

    if(!load_lookup< Import::GroupData >("groups"))
        return false;

    if(!load_lookup< Import::WeightData >("weights"))
        return false;

    if(!load_lookup< Import::IngredientData >("ingredients"))
        return false;

    if(!load_lookup< Import::CerealTypesData >("cereal_types"))
        return false;

 //   if(!load_lookup< Import::MilkTypesData >("milk_types"))
 //       return false;
    return true;
}

int
Epic::Client::Application::run()
{
    try
    {
        if(Epic::Database::created())
        {
            if(!load_tables())
            {
                Epic::Logging::error("Populating db tables failed\n");
                return EXIT_FAILURE;
            }
        }

        // parse data into the database
        if(!load_questionaire())
        {
            Epic::Logging::error("Questionaire processing not started due to insuffient input\n");
            Epic::Logging::note("Questionaire may be specifed as single file input or in a batch using a jobfile\n");
            return EXIT_FAILURE;
        }

        // sort out input / output
        Database::Report rs;
        if(m_output_file.empty())
        {
            rs.respondents(std::cout);
        }
        else
        {
            std::ofstream file(m_output_file.c_str());
            if(!file.is_open())
            {
                std::cerr << "unable to open output file " << std::endl;
                return EXIT_FAILURE;
            }
            rs.respondents(file);
            file.close();
        }
    }
    catch(std::runtime_error & e)
    {
        Epic::Logging::error(e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
