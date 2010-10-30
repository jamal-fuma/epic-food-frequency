#include "cmdline/Parser.hpp"
#include "client/Application.hpp"
#include "logging/Logger.hpp"
#include "config/Global.hpp"
#include "config/Quantity.hpp"

#define DEFAULT_CONFIG_FILE "conf/client.conf"

bool load_config(const std::string & config_file, Epic::Logging::Logger & log);
Epic::Logging::Logger logger(Epic::CmdLine::Parser::result_map_t & args);
int help();

Epic::Client::Options opts;

int
main(int argc, char **argv)
{
    Epic::CmdLine::Parser::result_map_t args;
    
    Epic::Client::Version ver;
    Epic::CmdLine::Parser cmdline = opts;
    
    if(!cmdline.parse(args,argc,argv))
    {
        help();
        return EXIT_FAILURE;
    }

    if(args.find("help") != args.end())
        return help();

    if(args.find("version") != args.end())
        return ver();

    Epic::Logging::Logger log = logger(args);

    if(args.find("config") == args.end())
    {
        // try a default config
        if(!load_config(DEFAULT_CONFIG_FILE,log))
            return help();

        // try a config under users home directory
    }
    // load config file specified on command line
    else if(!load_config(args["config"],log))
        return help();

    // direct logging to specifed file
    Epic::Client::Application app(log,args);
    return app.run();
}

bool
load_config(const std::string & config_file, Epic::Logging::Logger & log)
{
    try
    {
        // Load the config
        if(1 != ::utility_file_exists(config_file.c_str()))
        {
            log.error("Config file missing " + config_file);
            return false;
        }

        // application config
        Epic::Config::Global conf;
        conf.load(config_file);
        Epic::Config::Global::value_type cnf = Epic::Config::Global::config();
        if(cnf.find("base_dir") != cnf.end())
        {
            std::string base_dir = cnf["base_dir"];
            if(::chdir(base_dir.c_str()))
            {
                log.error("Failed to change working directory to : " 
                        + base_dir );
                return false;
            }
        }
 
        // nutrient quantity config
        if(cnf.find("nutrient_quantity") == cnf.end())
        {
            log.error("Nutrient Quantity Configuration file not listed in global config " + config_file);
            return false;
        }
        Epic::Config::Quantity quants;
        quants.load(cnf["nutrient_quantity"]);
    }
    catch(std::runtime_error & e)
    {
        log.error("Loading config file " + config_file + " failed, because " + e.what());
        return false;
    }
    return true;
}

int
help()
{
    // dump a usage message to stderr 
    return opts("\nEpic_Client <options> - where options is one or more of the following:");
}

Epic::Logging::Logger
logger( Epic::CmdLine::Parser::result_map_t & args)
{
     // direct logging to specifed file
    if(args.find("log-file") != args.end())
    {
        Epic::Logging::Logger file_log(args["log-file"]);
        return file_log;
    }
 
    // direct logging to stderr
    Epic::Logging::Logger stderr_log;
    return stderr_log;
}
