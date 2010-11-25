#include "Epic_lib.hpp"

#include "import/Import.hpp"

int
main(int argc, char **argv)
{
    std::string import  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/import/meals.csv";
    std::string conf    = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/client.conf";
    std::string schema  = "/home/me/workspace/clone/epic-food-frequency-0.0.1/sql/model.sql" ;
    std::string dbase   = "/home/me/workspace/clone/epic-food-frequency-0.0.1/build/test/foods.db" ;

    if(!Epic::Config::load(conf))
    {
        std::ostringstream ss;
        ss << "Config file missing " << conf << std::endl;
        Epic::Logging::error(ss.str());
        return EXIT_FAILURE;
    }

    // overwrite the schema variable listed in config file with the new version
    if(!Epic::Config::insert("schema",schema,true))
        return EXIT_FAILURE;

    // overwrite the database variable listed in config file with the new version
    if(!Epic::Config::insert("database",dbase,true))
        return EXIT_FAILURE;

    Epic::Database::connect();

    Epic::Import::CSVReader rdr;

    if(argc > 1)
    {
        import = argv[1] ;
    }

    if(!rdr.open(import))
    {
        return EXIT_FAILURE;
    }
    Epic::Import::str_vector_t v,h;

    Epic::Config::Config cnf;

    for(size_t line=0; (rdr.more_rows()); ++line)
    {
        if(rdr.read_row(v))
        {
            if(!line)
            {
                h = v;
                continue;
            }
            Epic::Import::str_vector_t::size_type end = v.size();
            for(Epic::Import::str_vector_t::size_type pos=0; pos != end; ++pos)
            {
                cnf.insert(h[pos],v[pos],true);
            }

            std::cout << cnf << std::endl <<std::endl;
        }
    }

    return EXIT_SUCCESS;
}

