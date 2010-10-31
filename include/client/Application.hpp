#ifndef EPIC_APPLICATION_HPP
#define EPIC_APPLICATION_HPP

#include "libUtility.h"

#include "client/Version.hpp"
#include "client/Options.hpp"
#include "logging/Logger.hpp"
#include "cmdline/Parser.hpp"

#include "import/Questionaire.hpp"
#include "dataset/QuestionStatement.hpp"

#include "dataset/Database.hpp"
#include "dataset/Schema.hpp"

#include "dataset/QuestionaireStatement.hpp"
#include "dataset/RespondentStatement.hpp"
#include "dataset/ResponseStatement.hpp"

#include "config/Global.hpp"

namespace Epic
{
    namespace Client
    {   
        typedef CmdLine::Parser::result_map_t cmdline_t;
        typedef Logging::Logger logger_t;

        class Application
        {
            enum FileType { InputFile=1, JobFile, Neither } ;

        public:
            Application(Client::logger_t & log,Client::cmdline_t & args) : 
                m_args(args), 
                m_logger(log),
                m_config_map(Epic::Config::Global::config()),
                m_status(Application::Neither)

            {
                // sort out input / output
                if(m_args.find("input") == m_args.end())
                {
                    std::string s("No input file specifed on command line\n");
                    if(m_args.find("jobfile") == m_args.end())
                    {
                        char replacement[] = "input";
                        m_logger.error(s);
                        s.replace(s.find(replacement),sizeof(replacement),"jobfile");
                        m_logger.error(s);
                    }
                    else
                    {
                        m_input_file = m_args.find("jobfile")->second;
                        m_status = Application::JobFile;
                    }
                }
                else
                {
                    m_input_file = m_args.find("input")->second;
                    m_status = Application::InputFile;
                }

            }
            
            int run();
            virtual ~Application() 
            {
                m_logger.trace("Shutting Down\n");
            }

            int
            help(void)
            {
                return m_options("Epic_Client");
            }

            int
            version(void)
            {
                return m_version();
            }

            bool
            create_tables(Database::DBConnection & db);
           
                        
            bool
            load_questionaire(Database::DBConnection & db)
            {
                Import::QuestionaireData qd(db);
                Database::QuestionaireInsertStatement qs(db);
                sqlite3_int64 row;
                switch(m_status)
                {
                    case Application::InputFile :


                        qs.bind("epic",m_input_file);
                        qs.step();
                        qs.reset();
                        row = sqlite3_last_insert_rowid(db);

                        // need to tell the loader where to insert data;
                        //qd.m_questionaire_id = row;

                        if(!Import::load(m_input_file,qd))
                        {
                            m_logger.error("Loading Questionaire file " + m_input_file + "failed\n");
                            return false;
                        }
                        return qd.commit();
                        break;

                        // rewire jobfile processing
                    case Application::JobFile   :
                        std::cout << "Job file specifed as " << m_input_file;
                        break;
                    default:
                        return false;
                }
                return true;
            }

            bool
            configured_value(const std::string & key,std::string & s)
            {
                Epic::Config::Global::const_iterator it = m_config_map.find(key);

                if(it != m_config_map.end())
                {
                    s = it->second;
                    return true;
                }
                return false;
            }

           private:
            
            // load lookup tables
            bool
            load_tables(Database::DBConnection & db);
          
            // generic loader for lookup tables
            template <class Data>
            bool load_lookup(Database::DBConnection & db,const std::string & config_key);

          
            Options             m_options;
            Version             m_version;
            Client::cmdline_t & m_args;
            Logging::Logger   & m_logger;
            std::string         m_config_file;
            std::string         m_input_file;
            std::string         m_output_file;
            Epic::Config::Global::value_type & m_config_map;
            Application::FileType m_status;

        };
    }// Epic::Client
}//Epic
#endif /* ndef EPIC_APPLICATION_HPP */
