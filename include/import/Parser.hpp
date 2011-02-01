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

#ifndef EPIC_IMPORT_PARSER_HPP
#define EPIC_IMPORT_PARSER_HPP

#include "boost/spirit/include/classic_core.hpp"
#include "boost/spirit/include/classic_file_iterator.hpp"

#include "../include/import/LineInfo.hpp"
#include "../include/import/NewLine.hpp"
#include "../include/import/InsertValue.hpp"

namespace Epic
{
    namespace Import
    {
        template<class IteratorT>
        struct CSVParser :
            public boost::spirit::classic::grammar< CSVParser<IteratorT> >
        {
            LineInfo<IteratorT> & m_line_info; // newline counter

            str_vector_t & m_row; // each iteration fills this container

            CSVParser(
                    str_vector_t & vec,
                    LineInfo<IteratorT>& info
                    ) :
                m_line_info(info),
                m_row(vec)
            {
            }

            // define the grammer accepted by the scanner
            template <typename ScannerT>
            struct definition
            {
                // rules defining grammer
                boost::spirit::classic::rule<ScannerT>
                csv, val, quoted_val, naked_val;

                // wire up sematic actions in grammer definition
                definition(const CSVParser<IteratorT>& self)
                {
                    // CSV rows consist of one or more comma delimited values
                    csv = val
                        >> *(',' >> val)
                        >> ( boost::spirit::classic::eol_p[
                                NewLine<IteratorT>(self.m_line_info)
                            ]
                        |
                            boost::spirit::classic::end_p
                            );

                    // CSV value are either quoted or unquoted
                    val = *boost::spirit::classic::blank_p
                        >> boost::spirit::classic::ch_p('\"')  >> quoted_val[
                                    InsertValue(self.m_row)
                            ] >> boost::spirit::classic::ch_p('\"')
                        >> *boost::spirit::classic::blank_p
                        | naked_val[
                                    InsertValue(self.m_row)
                        ];

                    // Quoted values may span multiple lines
                    // with two literal double quotes treated as an escaped double quote
                    quoted_val = *(boost::spirit::classic::eol_p[
                                    NewLine<IteratorT>(self.m_line_info)
                                ]
                            | ~boost::spirit::classic::ch_p('"')
                            |  boost::spirit::classic::str_p("\"\"")
                    );

                    // a naked values is a contiguous sequence
                    // of characters terminated by one of [ comma,double quote or newline ]
                    naked_val = *(  ~boost::spirit::classic::ch_p(',')
                                    & ~boost::spirit::classic::ch_p('\"')
                                    & ~boost::spirit::classic::ch_p('\n')
                                );
                }

                const
                boost::spirit::classic::rule<ScannerT> &
                start() const
                {
                    return csv;
                }
            }; // nested definition of grammer
        };

        template<typename IteratorT>
        boost::spirit::classic::parse_info<IteratorT>
        parse_csv(
                const IteratorT & begin,
                const IteratorT & end,
                str_vector_t & vec,
                LineInfo<IteratorT> & info
                )
        {
            CSVParser<IteratorT> csv(vec, info);
            return parse(begin,end, csv);
        }

    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_PARSER_HPP */
