#include "config/Global.hpp"
#include "libcsv/CSVReader.hpp"

#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#undef NDEBUG

int
main(int argc, char **argv)
{
    Epic::Import::CSVReader rdr;

    // load same config we ship to users
    std::string conf    =  DEFAULT_CONFIG_FILE;
    assert(Epic::Config::load(conf) 
            && "Config loading should not fail");
 
    // grab the contents of the meal food table, it's a valid but short csv file
    std::string import;
    assert(Epic::Config::find("meal",import) 
            && "A value for the meal food table should be present in config file");
 
    // get ready to slurp the contents of the csv file
    assert(rdr.open(import)
            && "Opening the import csv should work, right?");

    // am empty file is not a test break, just a failure;
    if(!rdr.more_rows() )
    {
        std::cout << "Lines imported: [" << 0 << "]\n";
        return EXIT_FAILURE;
    }

    // grab the header line
    std::vector< std::string > header;
    assert(rdr.read_row(header)
            && "Opening the first line of csv should work");
    
    size_t lines=1;
    std::vector< std::string > row;
    for(Epic::Config::Config cnf; rdr.more_rows(); ++lines)
    {
        // grab the next data line
        assert(rdr.read_row(row)
            && "read_row() should never fail if more_rows() is true");

        // insert name,value pairs into the config object
        std::vector< std::string >::size_type end = row.size();
        for(std::vector< std::string >::size_type pos=0; pos != end; ++pos)
        {
            cnf.insert(header[pos],row[pos],true);
        }
    }

    std::cout << "Lines imported: [" << lines << "]\n";
    return EXIT_SUCCESS;
}
