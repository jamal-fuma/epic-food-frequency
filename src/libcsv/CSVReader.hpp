/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EPIC_CSV_READER_HPP
#define EPIC_CSV_READER_HPP

//#include <iomanip>
//#include <fstream>
//#include "dataset/Database.hpp"

#include <string>
#include <vector>


namespace Epic
{
    namespace Import
    {
        // forward declaration
        class Context ;
        
        class CSVReader
        {
        public:
            CSVReader();
            CSVReader(const std::string & filename);
            ~CSVReader()
            { 
                close();
            }

            bool open(const std::string & filename);
            void close();
            bool more_rows();
            bool read_row( std::vector< std::string > & row);

            private:
            // non-copyable
            CSVReader(const CSVReader & rhs);
            CSVReader& operator=(const CSVReader & rhs);

            Epic::Import::Context *m_ctx;
            std::string            m_filename;
            bool                   m_open;
        };
    } // Epic::Import
} // Epic

#endif /* EPIC_CSV_READER_HPP */



