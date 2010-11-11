#include "logging/Logger.hpp"

void Epic::Logging::error(const std::string & str)
{
    Epic::Pattern::Singleton< Logger >::instance().error(str);
}

void Epic::Logging::trace(const std::string & str)
{

    Epic::Pattern::Singleton< Logger >::instance().trace(str);
}

void Epic::Logging::note(const std::string & str)
{
    Epic::Pattern::Singleton< Logger >::instance().note(str);
}

bool Epic::Logging::open(const std::string & filename)
{
    try
    {
        Epic::Pattern::Singleton< Logger >::instance().open(filename);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

