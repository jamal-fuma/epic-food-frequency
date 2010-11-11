#ifndef EPIC_LOGGER_HPP
#define EPIC_LOGGER_HPP

#include <stdio.h>
#include <string>
#include <stdexcept>
#include "pattern/Singleton.hpp"

namespace Epic
{
    namespace Logging
    {
        class Logger
        {
            FILE *m_fp;
            bool m_trace;
            bool m_close_on_exit;
            std::string m_filename;

        public:
           
            Logger() : m_fp(stderr),
                m_close_on_exit(false), m_filename("stderr")
            {
            }

            void
            open(const std::string & filename)    
            {
                m_fp = fopen(filename.c_str(),"w");
                if(!m_fp)
                {
                    throw std::runtime_error("Unable to open log file" + filename);
                }
                m_filename = filename;
                m_close_on_exit = true;
            }

            ~Logger()
            {
                close();
            }

            void
            trace(const std::string & msg) const
            {
                fprintf(m_fp,"Trace: %s",msg.c_str());
            }
            
            void
            error(const std::string & msg) const
            {
                fprintf(m_fp,"Error: %s",msg.c_str());
            }
            
            void
            note(const std::string & msg) const
            {
                fprintf(m_fp,"Note: %s",msg.c_str());
            }
        
        protected:
            void
            close()
            {
                if(m_close_on_exit)
                {
                    if(m_fp && m_fp != stderr)
                    {
                        fclose(m_fp);
                    }
                }
                m_fp = NULL;
            }
        };

        // wrappers for the log singleton
        bool open(const std::string & filename);
        void error(const std::string & str);
        void trace(const std::string & str);
        void note(const std::string & str);

    } // Epic::Logging
} // Epic

#endif /* ndef EPIC_LOGGER_HPP */
