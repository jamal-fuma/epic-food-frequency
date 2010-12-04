#ifndef EPIC_IMPORT_INSERT_VALUE_HPP
#define EPIC_IMPORT_INSERT_VALUE_HPP

#include <string>
#include <vector>
#include <algorithm>
#include "Epic_lib.hpp"

namespace Epic
{
    namespace Util
    {
        // front
        inline std::string & rtrim(std::string & s) {
            s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
            return s;
        }

        // back
        inline std::string & ltrim(std::string & s) {
            s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
            return s;
        }

        // front and back
        inline std::string & trim(std::string & s) {
            return ltrim(rtrim(s));
        }
        
        // return copyfront and back
        inline std::string c_trim(const std::string & s) {
            std::string cpy(s);
            return trim(cpy);
        }


        class Token
        {
            public:
                Token(const std::string & s) : m_tokens(NULL),m_count(0),m_ptr(strdup(s.c_str())), m_len(s.size()) { }

                Token & split(int ch){
                    if(m_ptr)
                    {
                        if(m_tokens)
                        {
                            reset();
                        }
                        m_tokens = utility_split(&m_count,m_ptr,ch);
                    }
                    return *this;
                }

                void reset()
                {
                    if(m_tokens)
                    {
                        free(m_tokens[0]);
                        free(m_tokens);
                        m_tokens = NULL;
                        m_count = 0;
                    }
                }

                Token & values(std::vector< std::string > & values) {
                    if(m_tokens)
                    {
                        std::vector< std::string > tmp;
                        std::copy(m_tokens,m_tokens+m_count,std::back_inserter(tmp));
                        values.swap(tmp);
                    }
                    return *this;
                }

                ~Token() {
                    if(m_ptr)
                    {
                        free(m_ptr);
                        m_ptr = NULL;
                    }
                    reset();
                }
            private:
                char    **m_tokens;
                char    *m_ptr;
                size_t   m_len;
                size_t   m_count;
        };


        class Line
        {
            public:
                Line(const std::string & s) : m_ptr(strdup(s.c_str())), m_len(s.size()) { }

                Line & convert(){
                    size_t cr=0;
                    utility_unixify(&cr,m_ptr,m_len);
                    return *this;
                }

                std::string str() {
                    if(m_ptr)
                    {
                        return std::string(m_ptr);
                    }
                    return std::string("");
                }

                ~Line() {
                    if(m_ptr)
                    {
                        free(m_ptr);
                        m_ptr = NULL;
                    }
                }
            private:
                char    *m_ptr;
                size_t   m_len;
        };

    } // Epic::Util

    namespace Import
    {
        typedef std::vector<std::string> str_vector_t;

        // wrapper that appends tokens into a container
        struct InsertValue
        {
            // container of std::string
            str_vector_t & m_tokens;

            InsertValue( str_vector_t & vec ) : m_tokens(vec) { }

            // all post-processing on tokens occurs here
            template<class IteratorT>
            void
            operator()( IteratorT begin, IteratorT end) const
            {
                std::string s(begin, end);
                std::string::size_type pos(0);

                // fold a rfc4180 literal double quote ("") into
                // a single literal double quote
                pos = s.find("\"\"", pos);
                for(; pos != std::string::npos;  pos = s.find("\"\"", pos))
                {
                    ++pos; // move past first double quote character
                    s.erase(pos, 1);
                }

                // need to remove carriage returns
                pos = s.find('\r',0);
                for(; pos != std::string::npos; pos = s.find('\r',pos+1) )
                {
                    s.erase(pos,1);
                }

                // need to remove leading/trailing whitespace
                Epic::Util::trim(s);
             //   s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
                m_tokens.push_back(s);
            }
        };
    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_INSERT_VALUE_HPP */
