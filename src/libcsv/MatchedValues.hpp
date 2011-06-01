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

#ifndef EPIC_CSV_MATCHED_VALUES_HPP
#define EPIC_CSV_MATCHED_VALUES_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <stddef.h>

#include "libhelper/Logger.hpp"
namespace Epic
{
    namespace Import
    {
        struct MatchedValues
        {
            template <typename Container> 
                bool operator()(
                        const std::string & name,
                        const Container & expected,
                        const Container & actual)
                {
#if(0)                   
                    if(insufficent_values_for_comparison(
                                name,
                                expected.size(),
                                actual.size()
                                ))
                    {
                        return false;
                    }

                    typedef typename Container::const_iterator Iterator;
                    std::pair< Iterator, Iterator > diff;

                    diff = std::mismatch(
                            expected.begin(),expected.end(),
                            actual.begin()
                            );

                    if(mismatched_values_present(
                                name,
                                std::distance(expected.begin(),diff.first),
                                diff
                                ))
                    {
                        return false;
                    }
#endif
                    return true;
                }

            bool 
                insufficent_values_for_comparison(
                        const std::string & name,
                        size_t expected_size,
                        size_t actual_size)
                {
                    bool insufficent(actual_size < expected_size);
                    if(insufficent)
                    {
                        Epic::Logging::Error().log() 
                            << "Unexpected number of fields in " << name << " import file," 
                            << " expected "   << expected_size << " field(s)" 
                            << " got " << actual_size << " field(s)" ;
                    }
                    return insufficent;

                }

            template <typename T>
                bool mismatched_values_present(
                        const std::string & name,
                        const typename T::difference_type & position,
                        std::pair<T,T> & diff
                        )
                {

                    bool mismatched(*diff.first != *diff.second);

                    if(mismatched)
                    {
                        Epic::Logging::Error().log()  
                            << "Unexpected fieldname for field at position: " 
                            << position  << "\n"
                            << " of " << name << " import file," 
                            << " expected " << *diff.first << " got " << *diff.second 
                            << " at this position instead\n";
                    }
                    return mismatched;
                }
        };
    } // Epic::Import
} // Epic

#endif /* EPIC_CSV_MATCHED_VALUES_HPP */



