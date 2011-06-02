#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "dao/Portion.hpp"
#include "config/Global.hpp"

void test_creating_a_portion();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();
    test_creating_a_portion();
    return EXIT_SUCCESS;
}

void
test_creating_a_portion()
{
    Epic::DAO::Portion portion ;
    portion.set_amount(1.0);
    assert(portion.save() && "Saving Portion should not fail");
}
