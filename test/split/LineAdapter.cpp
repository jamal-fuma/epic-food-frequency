/*
 * =====================================================================================
 *
 *       Filename:  LineAdapter.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/23/11 18:06:01
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jamal M. Natour (fuma), jamal.natour@fumasoftware.com
 *        Company:  FumaSoftware
 *
 * =====================================================================================
 */

#include "LineAdaptor.hpp"

#include <algorithm>
#include <iterator>
#include <iostream>

    struct extract_c_pointer
    {
        char * operator()(std::string s )
        {
            return const_cast<char *>(s.c_str());
        }
    };
    
    struct empty_string
    {
        bool operator()(std::string s)
        { 
            return Epic::Util::trim(s).empty();
        }
    };

    LineAdaptor::LineAdaptor(const std::string & str)
    {
        std::stringstream ss(str);
        
        // extract a vector of std::string from the stream
        std::copy(
            std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(),
            std::back_inserter(m_strings),
        );
        
        copy_if_not_empy();
    }
 
    LineAdaptor::LineAdaptor(const std::vector<std::string> & svec) 
      :
        m_strings(svec)
    {
        copy_if_not_empy();
   }


  void LineAdapter::copy_if_not_empy(void)
  {
        // remove any whitespace only words
        std::erase(
            std::remove_if(
              m_strings.begin(), m_strings.end(),
              empty_string
            )
            m_strings.end()
        );
        
        // we need an C-style array of shallow copies of the 
        // pointers to memory owned by RCSP implementation
        std::vector<char *> c_strings(strings.size()+1);
        std::transform(
            m_strings.begin(),m_strings.end(),
            c_strings.begin(),
            extract_c_pointer()
        );
        m_c_strings.assign(c_strings);
  }
