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

#include "libcsv/CSVReader.hpp"

#include "import/Context.hpp"

//#include "import/Parser.hpp"
//

#include "libhelper/Logger.hpp"

namespace detail
{
    struct allocate_file_context
    {
        Epic::Import::Context *
            operator()(const std::string & filename)
            {
                Epic::Import::Context::iterator_t begin(filename);
                if(!begin)
                {
                    Epic::Logging::Error().log() 
                        << "Unable to open file: " 
                        << filename;
                    return NULL;
                }

                return new Epic::Import::Context(begin, begin.make_end());
            }
    };
} // detail namespace


Epic::Import::CSVReader::CSVReader() 
    : m_ctx(NULL),
      m_open(false) 
{ 

}

Epic::Import::CSVReader::CSVReader(
        const std::string & filename
        )
    :   m_ctx(NULL),
        m_filename(filename)
{
        m_open = open(filename) ;
}

bool
Epic::Import::CSVReader::open(
         const std::string & filename
         ) 
{
    close();

    m_ctx = detail::allocate_file_context()(filename);
    if(!(m_open = ( (m_ctx) != NULL)))
    { 
        Epic::Logging::Error().log()
            << "Unable to allocate memory for parser context: " ;
    }
    else
    {
        m_filename = filename;
    }
    return m_open;
}

void
Epic::Import::CSVReader::close()
{
    if(m_ctx)
    {
        delete m_ctx;
        m_ctx = NULL;
        m_open = false;
    }
}

bool
Epic::Import::CSVReader::more_rows()
{
    return (m_open = m_ctx->more_rows());
}

bool
Epic::Import::CSVReader::read_row(
        std::vector< std::string > & row
        ) 
{
    if(!m_open)
        return false;

    return m_ctx->read_row(row);
}



