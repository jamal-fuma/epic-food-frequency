#ifndef EPIC_CLIENT_HPP
#define EPIC_CLIENT_HPP

#include "Epic.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include "dataset/Dataset.hpp"
#include "dataset/Database.hpp"
#include "parser/Parser.hpp"
#include "import/Import.hpp"
#include "import/Meal.hpp"
#include "import/Ingredient.hpp"
#include "logging/Logger.hpp"

#define DEFAULT_CONFIG_FILE "lookups.conf"
namespace Epic 
{
    namespace Client
    {
        template <typename Opts, typename Ver>
            class Application
            {
                Opts m_options;
                Ver  m_version;

                public:
                Application() {}
                virtual ~Application() {}

                int
                    help(void)
                    {
                        fprintf(stderr,"Usage %s\n", m_options.help());
                        return EXIT_SUCCESS;
                    }

                int
                    version(void)
                    {
                        fprintf(stderr,"Version %d.%d\nLicence:\n%s\n",
                                m_version.m_major,
                                m_version.m_minor,
                                m_version.m_license.c_str());
                        return EXIT_SUCCESS;
                    }

                    Client::Params
                    params(int argc,char **argv)
                    {
                        return Client::Params(m_options,argc,argv);
                    }

                protected:
                virtual int
                    run(Client::Params & params) = 0;
            };

      
        struct FFQClientPolicy
        {
            typedef  Client::Options        Opts;
            typedef  Client::Version        Ver;
            typedef  Logging::Logger     Logger;
        };

        
        template <typename Policy = FFQClientPolicy>
            class FFQClient : public Application<typename Policy::Opts, typename Policy::Ver>
        {
            public:
                typedef Application<typename Policy::Opts, typename Policy::Ver> Base;
                typedef typename Policy::Logger Logger;

                FFQClient()
                    : Base() {}

                static int
                    run(int argc, char **argv)
                    {
                        FFQClient app;
                        Client::Params params(app.params(argc,argv));

                        if(params.valid())
                        {
                            if(params.present("help"))
                                return app.help();

                            if(params.present("version"))
                                return app.version();

                            return app.run(params);
                        }
                        app.help();
                        return EXIT_FAILURE;
                    }

                int
                    run(Client::Params & params)
                    {
                        Commandline::Parser::result_map_t args = params;
                        Commandline::Parser::result_map_iterator_t it;

                        // handle arguments

                        it = args.find("log-file");
                        if(it != args.end())
                        {
                            if(!m_logger.to_logfile(it->second))
                            {
                                m_logger.write("Unable to direct logs to specifed file: " + it->second);
                                return EXIT_FAILURE;
                            }
                        }

                        std::string conf = DEFAULT_CONFIG_FILE;
                        it = args.find("config");
                        if(it == args.end())
                        {

                            m_logger.write("no config file specifed, using default " + conf);
                            if(1 != ::utility_file_exists(conf.c_str()))
                            {
                                m_logger.write("no config file and specifed, default config file missing " + conf);
                                Base::help();
                                return EXIT_FAILURE;
                            }
                        }
                        else
                        {
                            conf = it->second;
                        }

                        ConfigLoader loader(conf);
                        std::map<std::string,Config> configs_by_name;
                        std::map<long,Import::Meal>  m_meals_by_line;
                        if(loader.load(configs_by_name))
                        {
                            std::map<std::string,Config>::iterator it, end;
                            it = configs_by_name.begin();
                            end = configs_by_name.end();

                            if(args.find("verbose") != args.end())
                            {
                                std::ostringstream ss ("Config loaded\n");
                                for(; it != end; ++it)
                                {
                                    ss << it->second ;
                                }
                                m_logger.write(ss.str());
                            }
                            
                            it = configs_by_name.find("database");
                            if(it == configs_by_name.end())
                            {
                                m_logger.write("No value found for database in config file");
                                return EXIT_FAILURE;
                            }

                            if(args.find("verbose") != args.end())
                            {
                                m_logger.write("Database file specified in config file as:" + it->second);
                            }

                            return EXIT_SUCCESS;
                        }
                        m_logger.write("loading failed");
                        return EXIT_FAILURE;
                    }

                Logger m_logger;
        };
    }
}
#endif /*ndef EPIC_CLIENT_HPP */

