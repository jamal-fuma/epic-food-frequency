#ifndef EPIC_IMPORT_LINEINFO_HPP
#define EPIC_IMPORT_LINEINFO_HPP

namespace Epic
{
    namespace Import
    {
         // wrapper to keep track of line information
        template<typename IteratorT>
        struct LineInfo
        {
            IteratorT m_line_iter;      // last iterator position
            size_t    m_line_number;    // last line processed

            LineInfo( IteratorT begin ) : m_line_iter(begin), m_line_number(1) { }

            // update line counter and last iterator position
            void
            operator()(IteratorT begin)
            {
                m_line_iter = begin;
                ++m_line_number;
            }
        };
    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_LINEINFO_HPP */
