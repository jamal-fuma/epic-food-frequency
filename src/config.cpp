#include "config/Global.hpp"
#include "logging/Logger.hpp"

void Epic::Config::Config::load(const std::string & filename)
{
    Epic::Config::Reader rdr(filename);
    if(!rdr.load(m_params))
        throw std::runtime_error("failed to load configuration from " + filename);
}

bool Epic::Config::Config::find(const std::string & key, std::string & dest)
{
    std::map<std::string,std::string>::const_iterator value = m_params.find(key);
    if(value != m_params.end())
    {
        dest = value->second;
        return true;
    }
    return false;
}

bool Epic::Config::load(const std::string & filename)
{
    try
    {
        Epic::Pattern::Singleton< Config >::instance().load(filename);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::find(const std::string & key, std::string & dest)
{
    try
    {
        bool ret = Epic::Pattern::Singleton< Config >::instance().find(key,dest);
        if(!ret)
        {
            std::ostringstream ss;
            ss << "Config file lacks value for '" << "schema" "'\n" ;
            Epic::Logging::error(ss.str());
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::Field::load(const std::string & filename)
{
    try
    {
        Epic::Pattern::Singleton< Field::FieldConfig >::instance().load(filename);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::Field::find(const std::string & key, std::string & dest)
{
    try
    {
        bool ret = Epic::Pattern::Singleton< Field::FieldConfig >::instance().find(key,dest);
        if(!ret)
        {
            std::ostringstream ss;
            ss << "Config file lacks value for '" << "schema" "'\n" ;
            Epic::Logging::error(ss.str());
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::Quantity::load(const std::string & filename)
{
    try
    {
        Epic::Pattern::Singleton< Quantity::QuantityConfig >::instance().load(filename);
        return true;
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::Quantity::find(const std::string & key, std::string & dest)
{
    try
    {
        bool ret = Epic::Pattern::Singleton< Quantity::QuantityConfig >::instance().find(key,dest);
        if(!ret)
        {
            std::ostringstream ss;
            ss << "Config file lacks value for '" << "schema" "'\n" ;
            Epic::Logging::error(ss.str());
            return false;
        }
    }
    catch(...)
    {
        return false;
    }
}
