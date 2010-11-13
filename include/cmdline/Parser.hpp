#ifndef EPIC_CMDLINE_PARSER_HPP
#define EPIC_CMDLINE_PARSER_HPP

#include <map>
#include <string>       // std::string
#include <string.h>     // strcmp
#include <sstream>      // ostringstream
#include <stdio.h>
#include <iomanip>      //setw and friends

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

        class Parser
        {
        public:
            // map option names to option values, unamed values are give the $n syntax for names
            typedef std::map<std::string, std::string>  result_map_t;
            typedef result_map_t::iterator              result_map_iterator_t;
            typedef result_map_t::const_iterator        result_const_iterator_t;

            // map names to options
            typedef std::multimap<std::string, Option>  option_map_t;
            typedef option_map_t::iterator              option_map_iterator_t;
            typedef option_map_t::const_iterator        option_map_const_iterator_t;

            // map arguments to the position they occur
            typedef std::multimap<std::string, int>     order_map_t;
            typedef order_map_t::iterator               order_map_iterator_t;
            typedef order_map_t::const_iterator         order_map_const_iterator_t;

        private:
            // application name
            std::string m_name;

            // map an option to its parameter
            option_map_t m_opts_map;

            // map an argument to its position
            order_map_t  m_pos_map;

        public:
            Parser(){}

            bool
            add_option(
                    const char *name,
                    int short_opt,
                    bool has_value=false,
                    const char *long_opt=NULL
                    )
            {
                if(!name)
                    return false;

                Option opt;
                opt.m_long_opt  = (!long_opt) ? name : long_opt ;
                opt.m_short_opt = short_opt;
                opt.m_has_value = has_value;

                m_opts_map.insert(std::make_pair(name,opt));
                return true;
            }

            bool
            parse_long_options(result_map_t & dest, int argc, char **argv, int & i)
            {
                char *arg = ((argv[i])+2);
                option_map_iterator_t iter = m_opts_map.begin();
                option_map_iterator_t end  = m_opts_map.end();

                // see if we have a match for this argument
                for(; iter != end; ++iter)
                {
                    if(iter->second.m_long_opt == arg )
                    {
                        const char *value = "true";
                        // long options
                        if(!iter->second.m_has_value)
                        {
                            dest[iter->first] = value;
                            return true;
                        }
                        else if( (iter->second.m_has_value) && (i+1 < argc))
                        {
                            value = argv[++i]; // modifiy i by ref in calling code
                            dest[iter->first] = value;
                            return true;
                        }
                        else if( iter->second.m_has_value)
                        {
                            fprintf(stderr,"Error: found long opt --%s: %s\n",
                                    arg,"required value missing");
                            return false;
                        }
                    }
                }

                fprintf(stderr,"Error: found unknown long opt --%s: \n",arg);
                return false;
            }

            bool
            parse_short_options(
                    result_map_t & dest,
                    int argc,
                    char **argv,
                    int & i
            )
            {
                char *arg = ((argv[i])+1);
                option_map_iterator_t iter = m_opts_map.begin();
                option_map_iterator_t end  = m_opts_map.end();

                // see if we have a match for this argument
                for(; iter != end; ++iter)
                {
                    if(iter->second.m_short_opt == *arg )
                    {
                        const char *value = "true";
                        if(!iter->second.m_has_value)
                        {
                            dest[iter->first] = value;
                            return true;
                        }
                        else if( (iter->second.m_has_value) && (i+1 < argc))
                        {
                            value = argv[++i]; // modifiy i by ref in calling code
                            dest[iter->first] = value;
                            return true;
                        }
                        else if( iter->second.m_has_value)
                        {
                            fprintf(stderr,"%s missing argument to option --%s: \n",
                                    m_name.c_str(),iter->second.m_long_opt.c_str());
                            return false;
                        }
                    }
                }

                fprintf(stderr,"unknown option -%s: \n",arg);
                return false;
            }

            std::string
            describe() const
            {
                option_map_const_iterator_t end = m_opts_map.end();
                std::ostringstream ss ;
                ss << "\tUsage :" << std::endl;

                for(option_map_const_iterator_t i = m_opts_map.begin(); i != end; ++i)
                {
                    ss << "\t\t" << "(-" << static_cast<unsigned char>(i->second.m_short_opt) << ") \t --" << i->second.m_long_opt << std::endl;
                }
                ss << std::endl;
                return ss.str();
            }

            bool
            parse(
                result_map_t & dest,
                int argc,
                char **argv
                )
            {
                bool positional = false;

                m_name = argv[0];

                // positional unnamed arguments
                for(int i = 1; i<argc; ++i)
                {
                    int pos = 0;
                    // all remaining arguments are treated
                    // as purely positional following this terminator
                    if(!strcmp("--",argv[i]))
                    {
                        positional = true;
                        continue;
                    }

                    if(!positional && ('-' == argv[i][0] && '\0' != argv[i][1]))
                    {
                        if('-' == argv[i][1])
                        {
                            if(!parse_long_options(dest,argc,argv,i))
                                return false;
                        }
                        else
                        {
                            if(!parse_short_options(dest,argc,argv,i))
                                return false;
                        }
                    }

                    else
                    {
                        // consume positional arguments
                        std::ostringstream ss;
                        ss.imbue(std::locale::classic());
                        ss << "$" << ++pos ;
                        dest[ss.str()] = argv[i];
                    }
                }
                return true;
            }
        };

    }// Epic::CmdLine

}//Epic

#endif /* ndef EPIC_CMDLINE_PARSER_HPP */
