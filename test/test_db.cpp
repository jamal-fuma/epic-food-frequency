#include "Epic_lib.hpp"

#include "dataset/Statement.hpp"

int
main(int argc, char **argv)
{
    std::string conf    = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";
    std::string schema  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/sql/model.sql" ;
    std::string dbase   = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/test/foods.db" ;

    if(!Epic::Config::load(conf))
    {
        std::ostringstream ss;
        ss << "Config file missing " << conf << std::endl;
        Epic::Logging::error(ss.str());
        return EXIT_FAILURE;
    }

    // overwrite the schema variable listed in config file with the new version
    if(!Epic::Config::insert("schema",schema,true))
        return EXIT_FAILURE;

    // overwrite the database variable listed in config file with the new version
    if(!Epic::Config::insert("database",dbase,true))
        return EXIT_FAILURE;

    Epic::Database::connect();

#if(0)
    Epic::Database::Statement sql ("select * from meals");

    int rc;
    for( rc = sql.step(); (SQLITE_ROW == rc); rc = sql.step() )
    {
        int max = sql.column_count();
        for(int i=0; i<max; ++i)
        {
            std::cout << sql.column_text(i) << " ";
        }
        std::cout << std::endl;
    }
#endif
    return EXIT_SUCCESS;
}

