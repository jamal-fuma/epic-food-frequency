#ifndef EPIC_CMDLINE_OPTION_HPP
#define EPIC_CMDLINE_OPTION_HPP

#include <string>       // std::string

namespace Epic
{
    namespace CmdLine
    {
       struct Argument
        {
            std::string m_name;
            std::string m_value;
        };

        struct Option
        {
            std::string m_id;
            std::string m_long_opt;
            int         m_short_opt;
            bool        m_has_value;
        };

    }// Epic::CmdLine

}//Epic

#endif /* ndef EPIC_CMDLINE_OPTION_HPP */
