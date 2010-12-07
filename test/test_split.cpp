#include <algorithm>
#include <vector>
#include <iterator>

#include "Epic_lib.hpp"
#include "cmdline/Parser.hpp"

#include "import/Import.hpp"
#include <iostream>

bool split_string(const std::string & s, int ch, std::vector< std::string > & strings);
bool file_lines(const std::string & filename,std::vector< std::string > & lines);


class Jobfile
{
public:
    Jobfile() : m_begin(m_lines.begin()), m_end(m_lines.end()) {}

    bool load(const std::string & filename)
    {
         std::vector< std::string > lines;
         m_lines.swap(lines);
         if(!read_lines(filename,m_lines))
             return false;

         m_begin = m_lines.begin();
         m_end   = m_lines.end();
         return true;
    }
    
    bool more_lines() {
        return m_begin != m_end;
    }

    bool parse_line(Epic::CmdLine::Parser::result_map_t & args)
    {
        if(!more_lines())
            return false;

       Epic::CmdLine::Parser::result_map_t tmp;

       tmp.swap(args);

        Epic::CmdLine::Parser cmd;

        cmd.add_option("output",    'o',true);
        cmd.add_option("input",     'i',true);
        cmd.add_option("log-file",  'l',true);
        cmd.add_option("mapping",   'm',true);
        cmd.add_option("style",     's',true);

        std::vector<std::string> strings;

        Epic::Util::Token(*m_begin).split(' ').values(strings);
        if(strings.empty())
            return false;

        std::vector<const char *> c_strings(strings.size());

        for(std::vector<std::string>::const_iterator begin = strings.begin(), end = strings.end(); begin!= end; ++begin)
        {
            c_strings.push_back(begin->c_str());
        }
        
        ++m_begin;
        return cmd.parse(args,c_strings.size(),const_cast<char **>(&c_strings[0]));
    }
    
    bool read_lines(const std::string & filename, std::vector< std::string > & lines)
    {
        std::ifstream src(filename.c_str());
        if(!src.is_open())
            return false;

        for(std::string ln; (!src.eof()); )
        {
            std::getline(src,ln);

            ln = Epic::Util::Line( Epic::Util::trim(ln)).convert().str();

            if(!ln.empty())
            {
                lines.push_back(ln);
            }
        }
        return true;
    }

private:
    std::vector< std::string > m_lines;
    std::vector< std::string >::iterator m_begin;
    std::vector< std::string >::iterator m_end;
};



int
main(int argc, char **argv)
{

    Epic::CmdLine::Parser::result_map_t args;
    Jobfile jobfile;

    std::vector< std::string > lines;
    if(argc > 1)
    {
        if(jobfile.load(argv[1]))
            std::cerr << "loading jobfile failed" << std::endl;

        for(size_t jobs=0; jobfile.more_lines(); ++jobs)
        {
            if(jobfile.parse_line(args))
            {
                std::cout << "processing line: " << jobs << std::endl;
                // direct logging to specifed file
                if(args.find("log-file") != args.end())
                    std::cout << "specified logfile: " << args["logfile"] << std::endl;

                if(args.find("output") != args.end())
                    std::cout << "specified output: " << args["output"] << std::endl;

                if(args.find("input") != args.end())
                    std::cout << "specified input: " << args["input"] << std::endl;

                if(args.find("style") != args.end())
                    std::cout << "specified style: " << args["style"] << std::endl;

                std::cout << std::endl;
            }
        }
     }

    if(file_lines(argv[1],lines))
    {
        std::vector< std::string >::const_iterator ci  =  lines.begin();
        std::vector< std::string >::const_iterator end =  lines.end();
        for(size_t line=1; ci != end; ++ci,++line)
        {
            std::cout << "Line : " << line << " " << *ci << std::endl;
            std::vector< std::string > words;

            split_string(*ci,' ',words);

            std::vector< std::string >::const_iterator wi  =   words.begin();
            std::vector< std::string >::const_iterator wend =  words.end();
            for(size_t word=1; wi != wend; ++wi, ++word)
            {
                std::cout << "\tWord : " << word << " [" << *wi << "]" << std::endl;
            }
            std::cout << std::endl;

        }
    }    
    std::vector< std::string > actual, expected;

    split_string("foo;bar",';',actual);

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
    return 0;
}

bool file_lines(const std::string & filename, std::vector< std::string > & lines)
{
    std::ifstream src(filename.c_str());
    if(!src.is_open())
        return false;

    for(std::string ln; (!src.eof()); )
    {
        std::getline(src,ln);
        
        ln = Epic::Util::Line( Epic::Util::trim(ln)).convert().str();
        
        if(!ln.empty())
        {
            lines.push_back(ln);
        }
    }
    return true;
}

bool split_string(const std::string & s, int ch, std::vector< std::string > & strings)
{
    Epic::Util::Token(s).split(ch).values(strings);
    return (strings.size() > 0);
}


