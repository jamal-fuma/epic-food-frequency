#include "cmdline/Parser.hpp"
#include "client/Application.hpp"
#include "logging/Logger.hpp"
#include "config/Global.hpp"

static int help();

int
main(int argc, char **argv)
{
    try
    {
        Epic::Client::Version ver;
        Epic::CmdLine::Parser cmd;

        cmd.add_option("version",   'V');
        cmd.add_option("verbose",   'v');
        cmd.add_option("help",      'h');
        cmd.add_option("output",    'o',true);
        cmd.add_option("input",     'i',true);
        cmd.add_option("jobfile",   'j',true);
        cmd.add_option("log-file",  'l',true);
        cmd.add_option("config",    'f',true);
        cmd.add_option("mapping",   'm',true);
        cmd.add_option("style",     's',true);

        Epic::CmdLine::Parser::result_map_t args;
        if(!cmd.parse(args,argc,argv))
        {
            help();
            return EXIT_FAILURE;
        }

        if(args.find("help") != args.end())
            return help();

        if(args.find("version") != args.end())
            return ver();

        // direct logging to specifed file
        if(args.find("log-file") != args.end())
            Epic::Logging::open(args["log-file"]);

        // try a default config
        if(args.find("config") == args.end())
        {
            if(!Epic::Config::load(DEFAULT_CONFIG_FILE))
            {
                std::ostringstream ss;
                ss << "Config file missing " << DEFAULT_CONFIG_FILE << std::endl;
                Epic::Logging::error(ss.str());
                return EXIT_FAILURE;
            }
        }
        // load config file specified on command line
        else if(!Epic::Config::load(args["config"]))
        {
            std::ostringstream ss;
            ss << "Config file missing " << args["config"] << std::endl;
            Epic::Logging::error(ss.str());
            return EXIT_FAILURE;
        }
        
        // nutrient quantity config
        std::string config_file;
        if(!Epic::Config::find("nutrient_quantity",config_file))
        {
            Epic::Logging::error("Nutrient Quantity Configuration file not listed in application config\n");
            return EXIT_FAILURE;
        }
        
        if(!Epic::Config::Quantity::load(config_file))
        {
            Epic::Logging::error("Nutrient Quantity Configuration file not loaded\n");
            return EXIT_FAILURE;
        }

        Epic::Client::Application app;

        // sort out input / output
        if(args.find("input") != args.end())
        {
            app.set_input(args["input"]);
        }
        else if(args.find("jobfile") != args.end())
        {
            app.set_jobfile(args["jobfile"]);
        }
        else
        {
            Epic::Logging::error("No jobfile or input file specifed on command line\n");
            return EXIT_FAILURE;
        }
        
        if(args.find("output") != args.end())
            app.set_output(args["output"]);
    
        Epic::Database::connect();

        // all that for this
        return app.run();
    }
 
    catch(std::runtime_error & e)
    {
        std::ostringstream ss;
        ss << "Unable to continue, because " << e.what() << std::endl;
        Epic::Logging::error(ss.str());
        return EXIT_FAILURE;
    }
    catch(...)
    {   
        return EXIT_FAILURE;
    }
}

static int
help()
{
    std::cerr << "Epic_Client <options> - where options is one or more of the following:\n" ;
    std::cerr << "	Usage :\n" ;
    std::cerr << "		(-f) 	 --config\n" ;
    std::cerr << "		(-h) 	 --help\n" ;
    std::cerr << "		(-i) 	 --input\n" ;
    std::cerr << "		(-j) 	 --jobfile\n" ;
    std::cerr << "		(-l) 	 --log-file\n" ;
    std::cerr << "		(-m) 	 --mapping\n" ;
    std::cerr << "		(-o) 	 --output\n" ;
    std::cerr << "		(-s) 	 --style\n" ;
    std::cerr << "		(-v) 	 --verbose\n" ;
    std::cerr << "		(-V) 	 --version\n" ;
    std::cerr << std::endl;
    return EXIT_SUCCESS;
}
