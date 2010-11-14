#include "Epic_lib.hpp"

#include "dataset/Statement.hpp"

int
main(int argc, char **argv)
{
    std::string conf = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";

    if(!Epic::Config::load(conf))
    {
        std::ostringstream ss;
        ss << "Config file missing " << conf << std::endl;
        Epic::Logging::error(ss.str());
        return EXIT_FAILURE;
    }

    Epic::Database::connect();

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
    return (SQLITE_DONE == rc);

    return EXIT_SUCCESS;
}

