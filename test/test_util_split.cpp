#include <iostream>
#include <vector>

#include "config/Resource.hpp"
#include "config/Util.hpp"

template <typename T> void dump(T begin, T end)
{
    std::cout << "\n[";
    if(begin != end)
    {
        std::ostream_iterator< std::string > output( std::cout, "\n" );
        std::copy(begin,end-1,output);
        std::cout << *(end-1) << "]\n";
        std::cout << std::endl;
    }
}


int
main(int argc, char **argv)
{
    if(argc > 1)
    {
        std::string s;
        for( std::fstream fs(argv[1]); fs >> s; )
        {
            std::cout << "Got [" << s << "]\n";
            std::vector< std::string > svec;
            Epic::Util::split(svec,s,";");
            dump(svec.begin(),svec.end());
        }
    }

    return 0;
}
