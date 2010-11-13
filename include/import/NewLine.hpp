#ifndef EPIC_IMPORT_NEWLINE_HPP
#define EPIC_IMPORT_NEWLINE_HPP

#include "../include/import/LineInfo.hpp"

namespace Epic
{
    namespace Import
    {
         // wrapper that updates the line information
        // whenever an newline is consumed by parser
        template<typename IteratorT>
        struct NewLine
        {
            LineInfo<IteratorT> & m_line_info; // reference to newline counter

            NewLine( LineInfo<IteratorT>& info ) :
                m_line_info(info)
            {
            }

            // called whenever an newline is consumed by parser
            void
            operator()(
                    IteratorT begin, // unused
                    IteratorT end
                    ) const
            {
                // update line counter and iterator
                m_line_info(end);
            }
        };

    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_NEWLINE_HPP */
