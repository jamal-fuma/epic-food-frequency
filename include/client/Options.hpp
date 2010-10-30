#ifndef EPIC_CLIENT_OPTIONS_HPP
#define EPIC_CLIENT_OPTIONS_HPP

#include <stdio.h>
#include "../include/cmdline/Parser.hpp"

namespace Epic
{
    namespace Client
    {
        struct Options
        {
            CmdLine::Parser m_cmdline;
            Options()
            {
                m_cmdline.add_option("version",   'V');
                m_cmdline.add_option("verbose",   'v');
                m_cmdline.add_option("help",      'h');
                m_cmdline.add_option("output",    'o',true);
                m_cmdline.add_option("input",     'i',true);
                m_cmdline.add_option("jobfile",   'j',true);
                m_cmdline.add_option("log-file",  'l',true);
                m_cmdline.add_option("config",    'f',true);
                m_cmdline.add_option("mapping",   'm',true);
                m_cmdline.add_option("style",     's',true);

            }
            
            int
            help(const std::string & app) const
            {
                fprintf(stderr,"%s\n%s\n",app.c_str(), m_cmdline.describe().c_str());
                return EXIT_SUCCESS;
            }

            int
            operator()(const std::string & app) const
            {
                return help(app);
            }

            operator const CmdLine::Parser & () const
            {
                return m_cmdline;
            }
        };
    } // Epic::Client
} // Epic

#endif /* EPIC_CLIENT_OPTIONS_HPP */

