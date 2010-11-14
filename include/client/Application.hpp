#ifndef EPIC_APPLICATION_HPP
#define EPIC_APPLICATION_HPP

#include "logging/Logger.hpp"
namespace Epic
{
    namespace Client
    {
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
                Logging::trace("\nShutting Down\n");
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

            bool load_questionaire();

           private:

            // load lookup tables
            bool load_tables();

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
