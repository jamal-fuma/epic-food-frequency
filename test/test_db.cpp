#include "client/Application.hpp"
#include "cmdline/Parser.hpp"
#include "dataset/Statement.hpp"
#include <iostream>

bool
load_string( const std::string & filename, std::string & dest);

int
main(int argc, char **argv)
{
    Epic::CmdLine::Parser::result_map_t args;
    Epic::CmdLine::Parser cmdline;
    
    cmdline.add_option("version",   'V');
    cmdline.add_option("verbose",   'v');
    cmdline.add_option("help",      'h');

    cmdline.add_option("output",    'o',true);
    cmdline.add_option("input",     'i',true);
    cmdline.add_option("db",        'd',true);
    cmdline.add_option("jobfile",   'j',true);
    cmdline.add_option("log-file",  'l',true);
    cmdline.add_option("config",    'f',true);
    cmdline.add_option("mapping",   'm',true);
    cmdline.add_option("style",     's',true);

    if(!cmdline.parse(args,argc,argv))
    {
        std::cerr << "Bad arguments" << std::endl;
        return EXIT_FAILURE;
    }
    
    if(args.find("input") != args.end())
    {
            std::string data;
            if(!load_string(args["input"],data))
            {
                std::cerr << "unable to load data from input file " << std::endl;
                return EXIT_FAILURE;
            }

            std::ofstream file("test.txt");
            if(!file.is_open())
            {
                std::cerr << "unable to open ouput file " << std::endl;
                return EXIT_FAILURE;
            }
 
            file << data;
            file.close();

            std::cout << data;
#if(0)
            Epic::Database::DBConnection db ("nutrients.db");
            Epic::Database::Statement m_sql (db,str);
            
            // walk over the responses
            int rc = sqlite3_step(m_sql); 

            for( ; (SQLITE_ROW == rc); rc = sqlite3_step(m_sql) )
            {
                sqlite3_int64   id         = sqlite3_column_int64(m_sql,0);
                std::string     reference  = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,1) );
                std::string     group      = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,2) );
                std::string     title      = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,3) );
                std::string     response   = reinterpret_cast<const char *>(sqlite3_column_text(m_sql,4) );
            }
#endif

    }

    return 0;

}

bool
load_string( const std::string & filename, std::string & dest)
{
    size_t sz=0;
    char *errmsg=NULL;
    void *data = utility_slurp_with_sz(&sz,filename.c_str());
    if(!data)
        return false;

    std::string str (static_cast<char *>(data),sz);
    free(data);
    data = NULL;

    dest.clear();
    dest.assign(str);
    return true;
}

