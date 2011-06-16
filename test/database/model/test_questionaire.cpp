#include <stdlib.h>
#include <assert.h>

#include "libdao/Database.hpp"
#include "config/Global.hpp"

#include "dao/Questionaire.hpp"

void test_creating_a_questionaire_called_foo();
void test_finding_a_previously_created_questionaire_called_foo();

int
main(int argc, char **argv)
{
    std::string conf    =  DEFAULT_CONFIG_FILE;

    assert(Epic::Config::load(conf) && "Config loading should not fail");

    Epic::Database::connect();
    test_creating_a_questionaire_called_foo();
    test_finding_a_previously_created_questionaire_called_foo();
    return EXIT_SUCCESS;
}

void
test_creating_a_questionaire_called_foo()
{
    Epic::DAO::Questionaire questionaire ;
    questionaire.set_filename("foo");
    assert(questionaire.save() && "Saving Questionaire should not fail");
}

void
test_finding_a_previously_created_questionaire_called_foo()
{
    Epic::DAO::Questionaire questionaire = Epic::DAO::Questionaire::find_by_filename("foo");
    assert(questionaire.save() && "Finding previously created Questionaire called foo should not fail");
}
