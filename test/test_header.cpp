#include "libcsv/MatchedValues.hpp"
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <string>
#include <iostream>

int
main(int argc, char **argv)
{
    std::vector< std::string > actual, expected;

    // insufficent and incorrect fields
    actual.push_back("foo");

    expected.push_back("Foo");
    expected.push_back("Bar");

    if(Epic::Import::MatchedValues()("meals",expected,actual))
    {
        std::cout << "Passed" <<std::endl;
    }
    else
    {
        std::cerr << "Failed with error "<< std::endl;
    }

    // correct but misordered fields
    actual.clear();
    actual.push_back("Bar");
    actual.push_back("Foo");
    if(Epic::Import::MatchedValues()("meals",expected,actual))
    {
        std::cout << "Passed" <<std::endl;
    }
    else
    {
        std::cerr << "Failed with error "<< std::endl;
    }

    // correct fields
    actual.clear();
    actual.push_back("Foo");
    actual.push_back("Bar");
    if(Epic::Import::MatchedValues()("meals",expected,actual))
    {
        std::cout << "Passed" <<std::endl;
    }
    else
    {
        std::cerr << "Failed with error "<< std::endl;
    }

    return 0;
}
