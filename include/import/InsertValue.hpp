#ifndef EPIC_IMPORT_INSERT_VALUE_HPP
#define EPIC_IMPORT_INSERT_VALUE_HPP

#include <string>
#include <vector>

namespace Epic
{
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

                m_tokens.push_back(s);
            }
        };
    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_INSERT_VALUE_HPP */
