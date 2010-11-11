#ifndef EPIC_CONFIG_RESOURCE_HPP
#define EPIC_CONFIG_RESOURCE_HPP

#include <string>
#include "config/Global.hpp"

namespace Epic
{
    namespace Config
    {
        class Resource
        {
            std::string m_data;
            size_t m_size;
            void *m_ptr;
            public:
            Resource() : m_ptr(NULL), m_size(0) {}
            Resource(const std::string & filename) :m_ptr(NULL),m_size(0) 
            {
                if(!load(filename))
                {
                    throw std::runtime_error("Loading Resource from file failed");
                }
            }
            ~Resource() 
            {
                clear();
            }

            static bool
            load( const std::string & filename, std::string & dest)
            {
                try 
                {
                    Resource rs(filename);
                    rs.data(dest);
                    return true;
                }
                catch(...)
                {
                    return false;
                }
            }



            bool 
            load(const std::string & filename)
            {
                char *errmsg=NULL;

                clear();
                m_ptr = utility_slurp_with_sz(&m_size,filename.c_str());
                if(!m_ptr)
                {
                    m_size = 0;
                    return false;
                }

                return true;   
            }

            private:

            void
            data(std::string & dest)
            { 
                if(m_ptr)
                {
                    dest.assign(static_cast<char *>(m_ptr),m_size); 
                }
            };

            void
            clear()
            {
                if(m_ptr)
                {
                    free(m_ptr);
                    m_ptr = NULL;
                }
            }
        };
    } // Epic::Config
} // Epic
#endif /*ndef EPIC_CONFIG_RESOURCE_HPP */
