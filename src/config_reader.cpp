#include "config/Reader.hpp"

bool
Epic::Config::Reader::load(Epic::Config::Reader::value_type & dest)
{
    if(!m_conf.is_open())
        return false;

    std::string key,value;
    for(std::string ln; (!m_conf.eof()); )
    {
        std::getline(m_conf,ln);

        if(has_values(ln))
        {
            parse_line(ln,key,value);
            insert(key,value);
        }
    }
    dest = m_params;
    return true;
}

bool
Epic::Config::Reader::configured_value(const std::string & key,std::string & s)
{
    const_iterator it = m_params.find(key);
    if(it == m_params.end())
        return false;

    s = it->second;
    return true;
}

void
Epic::Config::Reader::strip_quotes(std::string & str)
{
    std::string::size_type pos = str.find("\"", 0);
    while(pos != std::string::npos)
    {
        str.erase(pos, 1);
        pos = str.find("\"", pos);
    }

    pos = str.find("'", 0);
    while(pos != std::string::npos)
    {
        str.erase(pos, 1);
        pos = str.find("'", pos);
    }
}

bool
Epic::Config::Reader::has_values(const std::string & line)
{
    return ( !line.empty()  &&
             line[0] != '#' &&
             line != "\n"   &&
             line != "\r"   &&
             line != "\r\n" );
}

void
Epic::Config::Reader::parse_line(
        const std::string & line,
        std::string & key,
        std::string & value)
{
    std::string::size_type pos = line.find('=');
    value      = "true";
    key        = line;

    // split name=value pairs
    if(pos != std::string::npos)
    {
        key   = line.substr(0,pos);
        value = line.substr(pos+1);
    }

    strip_quotes(key);
    strip_quotes(value);
}


