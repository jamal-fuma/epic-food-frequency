#include "config/Global.hpp"
#include "logging/Logger.hpp"

void Epic::Config::Config::load(const std::string & filename)
{
    Epic::Config::Reader rdr(filename);
    if(!rdr.load(m_params))
        throw std::runtime_error("failed to load configuration from " + filename);
}

bool Epic::Config::Config::insert(const std::string & key, const std::string & value, bool overwrite)
{
    iterator lower = m_params.lower_bound(key);
    if(lower != m_params.end() && !(m_params.key_comp()(key, lower->first)))
    {
        // exists and overwrite acceptable
        if(overwrite)
        {
            lower->second = value;
            return true;
        }
        // exists but not supposed to overwrite
        return false;
    }
    // key doesnt exists so use lower bound as insertion hint
    m_params.insert(lower, value_type(key, value));
    return true;
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
        return Epic::Config::Quantity::load();
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
            ss << "Config file lacks value for key : '" << key << "'" << std::endl ;
            Epic::Logging::error(ss.str());
            return false;
        }
        return ret;
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::insert(const std::string & key, const std::string & value, bool overwrite)
{
    try
    {
        bool ret = Epic::Pattern::Singleton< Config >::instance().insert(key,value,overwrite);
        if(!ret && !overwrite)
        {
            std::ostringstream ss;
            ss << "Failed to insert configuration value with key,value : ('" << key << "','" << value << "')" << std::endl ;
            Epic::Logging::error(ss.str());
            return false;
        }
        return ret;
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
            ss << "Config file lacks value for '" << key << "'\n" ;
            Epic::Logging::error(ss.str());
            return false;
        }
        return ret;
    }
    catch(...)
    {
        return false;
    }
}

bool Epic::Config::Quantity::load()
{
    try
    {
        // nutrient quantity config
        std::string config_file;
        if(!Epic::Config::find("nutrient_quantity",config_file))
        {
            Epic::Logging::error(
                "Nutrient Quantity Configuration file not listed in application config\n"
            );
            return false;
        }

        Epic::Pattern::Singleton< Quantity::QuantityConfig >::instance().load(config_file);
        return true;
    }
    catch(...)
    {
        Epic::Logging::error("Nutrient Quantity Configuration file not loaded\n");
        return false;
    }
}

bool Epic::Config::Quantity::find(const std::string & key, std::string & dest)
{
    try
    {
        bool ret = Epic::Pattern::Singleton< Quantity::QuantityConfig >::instance().find(key,dest);
        if(ret)
        {
            std::ostringstream ss;
            ss << "Mapped value for '" << key << "' as " << dest << "\n" ;
            Epic::Logging::note(ss.str());
        }
        return ret;
    }
    catch(...)
    {
        return false;
    }
}
