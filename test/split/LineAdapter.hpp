/*
 * =====================================================================================
 *
 *       Filename:  LineAdapter.hpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/23/11 17:57:10
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jamal M. Natour (fuma), jamal.natour@fumasoftware.com
 *        Company:  FumaSoftware
 *
 * =====================================================================================
 */

#include <vector>
#include <string>
 
    class LineAdaptor
    {
      public:
          LineAdaptor(const std::vector<std::string> & svec);
          LineAdaptor(const std::string & str);
          ~LineAdaptor();
          
          char ** begin() {
              return &m_c_strings[0]; 
          }
      
          size_t length(){
            return m_c_strings.size();
          }

      private:
        void copy_if_not_empy(void);
        
        std::vector<std::string> m_strings;
        std::vector<char *> c_strings;
    };


