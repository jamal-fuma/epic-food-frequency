#include <algorithm>

#include <vector>
#include <iterator>

#include "config/Util.hpp" 
#include "config/Resource.hpp" 
#include "cmdline/Parser.hpp"
#include "import/Import.hpp"

#include <iostream>

bool file_lines(const std::string & filename,std::vector< std::string > & lines);


namespace detail
{

    typedef Epic::CmdLine::Parser::result_map_t result_map_t;
 
    template <typename T> void dump(T begin, T end)
    {
        std::cout << "\n[";
        std::ostream_iterator< std::string > output( std::cout, "\n" );
        std::copy(begin,end,output);
        std::cout << "]\n";
        std::cout << std::endl;
    }

    struct dump_map_contents
    {
        void operator()( std::map<std::string,std::string>::value_type & it)
        {
           std::cout << "Key: " << it.first << "\n";
           std::cout << "Values" << "\n";
           std::cout << it.second << "\n";
        }
    };

    struct decode_arguments
    {
        result_map_t & operator()(result_map_t & args)
        {
            std::cout << "\n";
            // direct logging to specifed file
            if(args.find("log-file") != args.end())
                std::cout << "specified logfile: " << args["logfile"] << "\n";

            if(args.find("output") != args.end())
                std::cout << "specified output: " << args["output"] << "\n";

            if(args.find("input") != args.end())
                std::cout << "specified input: " << args["input"] << "\n";

            if(args.find("style") != args.end())
                std::cout << "specified style: " << args["style"] <<  "\n";

            std::cout << "\n";
            return args;
        }
    };
    
    struct extract_c_pointer
    {
        char * operator()(std::string s )
        {
            return const_cast<char *>(s.c_str());
        }
    };

    struct extract_argument
    {
        result_map_t operator()(std::vector<std::string> & strings)
        {
            // fill an C-style array of shallow copies of the pointers to memory owned by RCSP implementation 
            std::vector<char *> c_strings(strings.size()+1);
            std::transform(
                    strings.begin(),strings.end(),
                    c_strings.begin(),
                    extract_c_pointer()
                    );

            // parse an C-style array of shallow copies of the pointers to memory owned by RCSP implementation resulting an a argument map
            result_map_t args;
            Epic::CmdLine::Parser m_cmd;
            m_cmd.add_option("output",    'o',true);
            m_cmd.add_option("input",     'i',true);
            m_cmd.add_option("log-file",  'l',true);
            m_cmd.add_option("mapping",   'm',true);
            m_cmd.add_option("style",     's',true);
          
            
            if(!m_cmd.parse(args,strings.size(),&c_strings[0]))
            {
                std::cerr << "Parsing failed\n";
            }
            

            if(!args.empty())
            {
                std::cout << "dumping map contents\n";
                std::for_each(
                        args.begin(),args.end(),
                        dump_map_contents()
                        );

            }
            else
            {
                std::cout << "skip dumping map, it is empty\n";
            }
            return args;
        }
            
    };
        
    struct parse_ws_delimited_line
    {
        result_map_t operator()(std::string & str)
        {
            std::vector<std::string> svec;
            std::stringstream ss(str);
            std::string s;
            svec.push_back(std::string("app name"));
            while(ss >> s)
            {
               if( !Epic::Util::trim(s).empty())
                    svec.push_back(s);
            }
            //Epic::Util::split(svec,s,"");
            result_map_t args = extract_argument()(svec);
            return decode_arguments()(args);
        }
    };
  
} // detail

class Jobfile
{
public:
    Jobfile() {}
    
    bool arguments(const std::string & filename)
    {
        std::string s;
        Epic::Config::Resource::load(filename,s);
        
        std::stringstream ss(s);

        s.clear();

        std::vector<std::string> strings;
        while(std::getline(ss,s))
        {
            strings.push_back(s);
        }

        if(strings.empty())
            return false;
        
        std::vector<detail::result_map_t> configs(strings.size());
        std::transform(
                strings.begin(),strings.end(),
                configs.begin(),
                detail::parse_ws_delimited_line()
                );

        return true;
    }
};     



int
main(int argc, char **argv)
{
    if(argc > 1)
    {
        Jobfile().arguments(argv[1]);
    }
    else
    {
        std::cerr << "loading jobfile failed" << std::endl;
    }
#if(0)
    if(file_lines(argv[1],lines))
    {
        std::vector< std::string >::const_iterator ci  =  lines.begin();
        std::vector< std::string >::const_iterator end =  lines.end();
        for(size_t line=1; ci != end; ++ci,++line)
        {
            std::cout << "Line : " << line << " " << *ci << std::endl;
            std::vector< std::string > words;

            Epic::Util::split(words,*ci," ");

            std::ostream_iterator< std::string > output( std::cout, "|" );
            std::copy(words.begin(),words.end(),output);
            std::cout << std::endl;
      
            std::cout << std::endl;

        }
    }    
    std::vector< std::string > actual, expected;

    Epic::Util::split(actual,"foo;bar",";");

    // insufficent and incorrect fields
    expected.push_back("foo");
    expected.push_back("bar");

    if(Epic::Import::DBModel::same_header("meals",expected,actual))
    {
        std::cout << "Passed" <<std::endl;
    }
    
    {
        std::ostream_iterator< std::string > output( std::cout, " " );
        
        std::cout << std::endl << "Expected: ";
        std::copy(expected.begin(),expected.end(),output);
        std::cout << std::endl;
        
        std::cout << std::endl << "Actual: ";
        std::copy(actual.begin(),actual.end(),output);
        std::cout << std::endl;

        std::cout << std::endl << "Equality: ";
        std::cout << (std::equal(actual.begin(),actual.end(),expected.begin()) ? "true" : "false") ;
        std::cout << std::endl;
    }
#endif
    return 0;
}
