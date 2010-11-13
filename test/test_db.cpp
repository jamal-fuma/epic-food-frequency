#include "cmdline/Parser.hpp"
#include "client/Application.hpp"
#include "logging/Logger.hpp"
#include "config/Global.hpp"
#include "dataset/Statement.hpp"

int
main(int argc, char **argv)
{
    Epic::CmdLine::Parser::result_map_t args;
    args["config"] = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";

    if(!Epic::Config::load(args["config"]))
    {
        std::ostringstream ss;
        ss << "Config file missing " << args["config"] << std::endl;
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

