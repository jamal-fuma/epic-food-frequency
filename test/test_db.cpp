#include "client/Application.hpp"
#include "cmdline/Parser.hpp"
#include "dataset/Statement.hpp"
#include <iostream>

#include "config/Global.hpp"
#include "config/Resource.hpp"

int
main(int argc, char **argv)
{
    Epic::CmdLine::Parser::result_map_t args;
    Epic::CmdLine::Parser cmdline;
    
    cmdline.add_option("version",   'V');
    cmdline.add_option("verbose",   'v');
    cmdline.add_option("help",      'h');

    cmdline.add_option("output",    'o',true);
    cmdline.add_option("input",     'i',true);
    cmdline.add_option("db",        'd',true);
    cmdline.add_option("jobfile",   'j',true);
    cmdline.add_option("log-file",  'l',true);
    cmdline.add_option("config",    'f',true);
    cmdline.add_option("mapping",   'm',true);
    cmdline.add_option("style",     's',true);

    if(!cmdline.parse(args,argc,argv))
    {
        std::cerr << "Bad arguments" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string data;
    if(args.find("input") != args.end())
    {
        Epic::Config::Resource::load(args["input"],data);
        std::cout << "loaded from file: " << data << std::endl;
    }

    if(args.find("config") != args.end())
    {
        Epic::Config::load(args["config"]);
    }
    
    if(Epic::Config::find("tst",data))
        std::cout << "loaded from config: " << data << std::endl;

 //   Import::NutrientData importer(db);
  //  Import::load(value,importer);
    return 0;
}

