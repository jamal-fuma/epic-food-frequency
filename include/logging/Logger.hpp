/* This file is part of the libEpic library */

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

#ifndef EPIC_LOGGER_HPP
#define EPIC_LOGGER_HPP

#include <stdio.h>
#include <string>
#include <time.h>
#include <stdexcept>
#include "Singleton.hpp"
#include <sstream>
namespace Epic
{
    namespace Logging
    {
        class Logger
        {
            FILE *m_fp;
            bool m_close_on_exit;
            std::string m_filename;

        public:

            Logger() : m_fp(stderr),
                m_close_on_exit(false), m_filename("stderr")
            {
            }

            void
            open(const std::string & filename) {
                if(m_close_on_exit)
                {
                    close();
                    m_close_on_exit = false;
                }
                m_fp = fopen(filename.c_str(),"w");
                if(!m_fp)
                {
                    throw std::runtime_error("Unable to open log file" + filename);
                }
                m_filename = filename;
                m_close_on_exit = true;
            }

            ~Logger() {
                close();
            }

           
            void
            trace(const std::string & msg) {
                write("Trace",msg);
            }

            void
            error(const std::string & msg) {
                write("Error",msg);
            }

            void
            note(const std::string & msg) {
                write("Note ",msg);
            }

        protected:
            void
            close() {
                if(m_close_on_exit)
                {
                    if(m_fp && m_fp != stderr)
                    {
                        fflush(m_fp);
                        fclose(m_fp);
                    }
                }
                m_fp = NULL;
            }
            
            void write(const std::string & prefix, const std::string & message) {
                char current_time[] = "0000-00-00 00-00\0\0\0\0";
                time_t timer = time(NULL);
                strftime(current_time,sizeof(current_time),"%F %H:%M %P",localtime(&timer));
                fprintf(m_fp,"%s: %s: %s\n",current_time,prefix.c_str(),message.c_str());
            }

        };
        
       
        // wrappers for the log singleton
        bool open(const std::string & filename);
        void error(const std::string & str);
        void trace(const std::string & str);
        void note(const std::string & str);
        
        class Note
        {
            public:
                Note() {}
                std::ostringstream & log() {
                    return m_ss;
                }
                ~Note() {
                    Epic::Logging::note(m_ss.str());
                }

            private:
                std::ostringstream m_ss;
        };

        class Error
        {
            public:
                Error() {}
                std::ostringstream & log() {
                    return m_ss;
                }
                ~Error() {
                    Epic::Logging::error(m_ss.str());
                }

            private:
                std::ostringstream m_ss;
        };

        class Trace
        {
            public:
                Trace() {}
                std::ostringstream & log() {
                    return m_ss;
                }
                ~Trace() {
                    Epic::Logging::trace(m_ss.str());
                }

            private:
                std::ostringstream m_ss;
        };


    } // Epic::Logging
} // Epic

#endif /* ndef EPIC_LOGGER_HPP */
