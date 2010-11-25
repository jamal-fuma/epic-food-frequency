#include <algorithm>
#include <vector>
#include <iterator>

#include "Epic_lib.hpp"
#include "import/Import.hpp"
#include <iostream>

bool split_string(const std::string & s, int ch, std::vector< std::string > & strings);
bool file_lines(const std::string & filename,std::vector< std::string > & lines);

int
main(int argc, char **argv)
{
    std::vector< std::string > lines;
    if(argc > 1)
    {
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


