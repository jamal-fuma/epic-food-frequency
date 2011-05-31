/* This file is part of the libEpic client */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "Application.hpp"
#include "cmdline/Parser.hpp"
#include "libhelper/Logger.hpp"
#include "libdao/Database.hpp"
#include "license/GPL_License.hpp"

static int help();
static int version();
Epic::Client::Application::ReportType report_type(const std::string & name);

int
main(int argc, char **argv)
{
   
    try
    {
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
            return version();

        // direct logging to specifed file
        if(args.find("log-file") != args.end())
            Epic::Logging::open(args["log-file"]);

        // try a default config unless  config file specified on command line
        if(args.find("config") == args.end())
            args["config"] = DEFAULT_CONFIG_FILE;

        if(!Epic::Config::load(args["config"]))
        {
            Epic::Logging::Error().log() << "Config file missing " << args["config"] ;
            help();
            return EXIT_FAILURE;
        }

        Epic::Logging::Note().log() << "Starting database setup" ;
        Epic::Database::connect();
        Epic::Logging::Note().log() << "Completed database setup\n" ;

        Epic::Client::Application app;

        // sort out input / output
        if(args.find("output") != args.end())
            app.set_output(args["output"]);

        // output report type 
        if(args.find("style") != args.end())
        {
            app.set_report_type(report_type(args["style"]));
        }
        else
        {
            Epic::Logging::Note().log()  << "Defaulting to foods report as no report style specified \n" ;
        }

        if(args.find("input") != args.end())
        {
            app.set_input(args["input"]);
        }
#if(0)
        else if(args.find("jobfile") != args.end())
        {
            app.set_jobfile(args["jobfile"]);
        }
        else
        {
            Epic::Logging::Error().log() << "No jobfile or input file specifed on command line";
            help();
            return EXIT_FAILURE;
        }
#else
        else
        {
            Epic::Logging::Error().log() << "No Input file specifed on command line";
            help();
            return EXIT_FAILURE;
        }

#endif /* jobfile */

        // all that for this
        return app.run();
    }

    catch(std::runtime_error & e)
    {
        Epic::Logging::Error().log() << "Unable to continue, because "  << e.what() ;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        Epic::Logging::Error().log() << "Unable to continue " ;
        return EXIT_FAILURE;
    }
}

static int
help()
{
    std::cerr << "\nEpic_Client <options> - where options is one or more of the following:\n" ;
    std::cerr << "	Usage :" << std::endl;
    std::cerr << "		(-f) 	 --config (defaults to '" << DEFAULT_CONFIG_FILE << "')\n";
    std::cerr << "		(-h) 	 --help (shows this text)\n" ;
    std::cerr << "		(-i) 	 --input (input file to process)\n" ;
 //   std::cerr << "		(-j) 	 --jobfile\n" ;
    std::cerr << "		(-l) 	 --log-file\n" ;
//    std::cerr << "		(-m) 	 --mapping\n" ;
    std::cerr << "		(-o) 	 --output\n" ;
    std::cerr << "		(-s) 	 --style=[foods | meals | nutrients | spreadsheet] (defaults to 'foods')\n";
    std::cerr << "		(-v) 	 --verbose\n" ;
    std::cerr << "		(-V) 	 --version\n" ;
    std::cerr << std::endl;
    return EXIT_SUCCESS;
}

static int
version()
{
    std::cerr << "Epic_Client V " << PACKAGE_VERSION << std::endl;
    std::cerr << GPL_LICENSE_TEXT << std::endl;
    std::cerr << std::endl;
    return EXIT_SUCCESS;
}


Epic::Client::Application::ReportType 
report_type(const std::string & name)
{
    std::map<std::string, Epic::Client::Application::ReportType> report_types_by_name;

    report_types_by_name["foods"]           = Epic::Client::Application::Foods;
    report_types_by_name["meals"]           = Epic::Client::Application::Meals;
    report_types_by_name["nutrients"]       = Epic::Client::Application::Nutrients;
    report_types_by_name["spreadsheet"]     = Epic::Client::Application::Spreadsheet;

    std::map<std::string, Epic::Client::Application::ReportType>::iterator report_type_it = report_types_by_name.find(Epic::Util::c_trim(name));
    if(report_type_it != report_types_by_name.end())
        return report_type_it->second;

    Epic::Logging::Error().log() << "Unknown report type: " << name ;
    Epic::Logging::Note().log()  << "Defaulting to foods report " ;

    return report_types_by_name["foods"];
}

