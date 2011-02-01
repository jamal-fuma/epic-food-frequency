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

#ifndef EPIC_IMPORT_HPP
#define EPIC_IMPORT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include "dataset/Database.hpp"
#include "import/Parser.hpp"
#include "import/Context.hpp"
#include "logging/Logger.hpp"

namespace Epic
{
    namespace Import
    {
        template <class Data>
        bool
        load(std::string & filename, Data & data)
        {
            Context::iterator_t first(filename);
            if(!first)
                return false;

            Context ctx(first,first.make_end());
            return ctx.load(data);
        }

        struct DBModel
        {
            DBModel() {}

            virtual bool error(std::string & error_message)
            {
                Epic::Logging::Error().log() << error_message ;
                return false;
            }

            virtual ~DBModel(){}

            protected:
                virtual bool header(str_vector_t & names) = 0;
                virtual bool body(size_t line, str_vector_t & values) = 0;

            public:
                static bool
                same_header( const std::string  & name,
                        const std::vector< std::string > & expected,
                        const std::vector< std::string > & actual)
                {
                    // sufficent fields for format
                    if(actual.size() < expected.size())
                    {
                        Epic::Logging::Error().log() << "Unexpected number of fields in " << name << " import file," << " expected "   << expected.size() << " field(s)" << " got " << actual.size() << " field(s)" ;
                        return false;
                    }

                    std::vector< std::string >::const_iterator it,end;
                    it = expected.begin(); end = expected.end();

                    for(size_t pos=0; it != end; ++pos,++it)
                    {
                        if(*it != actual.at(pos))
                        {
                            Epic::Logging::Error().log()  << "Unexpected fieldname for field: " << (pos+1) << " of " << name << " import file," << " expected "   << *it << " got " << actual.at(pos) ;
                            return false;
                        }
                    }
                    return true;
                }

        };

        template<class T>
        struct ImportModel
        {
            T m_model;
            Database::Transaction m_transaction;
            ImportModel() {}

            // header
            bool operator()(str_vector_t & v) { return m_model.header(v); }

            // line
            bool operator()(size_t line, str_vector_t & v) { return m_model.body(line,v); }

            // error
            bool
            operator()(std::string & error_message) { return m_model.error(error_message) ; }

            bool
            commit()
            {
                return m_transaction.commit();
            }
        };

        class CSVReader
        {
        public:
            CSVReader() : m_open(false) { }

            CSVReader(const std::string & filename) :
                m_filename(filename), m_open(false) {
            }

            ~CSVReader() {
                reset();
            }

            bool open(const std::string & filename) {
                m_filename = filename;
                return open();
            }

            bool open() {
                if(m_open)
                {
                    reset();
                }

                Epic::Import::Context::iterator_t begin(m_filename);
                if(!begin)
                {
                    Epic::Logging::Error().log() << "Unable to open file: " << m_filename;
                    return false;
                }

                m_ctx = new Epic::Import::Context(begin, begin.make_end());
                if(!m_ctx)
                {
                    Epic::Logging::Error().log() << "Unable to allocate memory for parser context: " ;
                    return false;
                }
                m_open = true;
                return true;
            }

            void close() {
                reset();
            }

            bool more_rows() {
                return (m_open = m_ctx->more_rows());
            }

            bool read_row(str_vector_t & row) {
                if(!m_open)
                    return false;
                return m_ctx->read_row(row);
            }

            private:
            void reset()
            {
                if(m_ctx)
                {
                    delete m_ctx;
                    m_ctx = NULL;
                    m_open = false;
                }
            }

            Epic::Import::Context *m_ctx;
            std::string            m_filename;
            bool                   m_open;
        };


    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_HPP */


