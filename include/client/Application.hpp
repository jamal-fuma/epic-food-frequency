#ifndef EPIC_APPLICATION_HPP
#define EPIC_APPLICATION_HPP

#include "libUtility.h"

#include "client/Version.hpp"
#include "logging/Logger.hpp"
#include "cmdline/Parser.hpp"

#include "import/Questionaire.hpp"
#include "dataset/QuestionStatement.hpp"

#include "dataset/Database.hpp"
#include "dataset/Schema.hpp"

#include "dataset/QuestionaireStatement.hpp"
#include "dataset/RespondentStatement.hpp"
#include "dataset/ResponseStatement.hpp"

#include "config/Resource.hpp"
#include "config/Global.hpp"

namespace Epic
{
    namespace Client
    {
        typedef CmdLine::Parser::result_map_t cmdline_t;

        class Application
        {
            enum FileType { InputFile=1, JobFile} ;

        public:
            Application() :
                m_status(Application::InputFile)
            {
            }

            ~Application()
            {
                Logging::trace("Shutting Down\n");
            }


            void
            set_input(const std::string & filename)
            {
                m_input_file = filename;
                m_status = Application::InputFile;
            }

            void
            set_jobfile(const std::string & filename)
            {
                m_input_file = filename;
                m_status = Application::JobFile;
            }


            void
            set_output(const std::string & filename)
            {
                m_output_file = filename;
            }

            int run();

            bool
            load_questionaire()
            {
                Import::QuestionaireData qd;
                Database::QuestionaireInsertStatement qs;
                switch(m_status)
                {
                    case Application::InputFile :

                        qs.bind("epic",m_input_file);
                        qs.step();
                        qs.reset();

                        // need to tell the loader where to insert data;
                        //qd.m_questionaire_id = row;

                        if(!Import::load(m_input_file,qd))
                        {
                            Logging::error("Loading Questionaire file " + m_input_file + "failed\n");
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
           private:

            // load lookup tables
            bool
            load_tables();

            // generic loader for lookup tables
            template <class Data>
            bool load_lookup(const std::string & config_key);

            std::string         m_input_file;
            std::string         m_output_file;
            Application::FileType m_status;

        };
    }// Epic::Client
}//Epic
#endif /* ndef EPIC_APPLICATION_HPP */
