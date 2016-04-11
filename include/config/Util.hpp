#ifndef EPIC_UTIL_HPP
#define EPIC_UTIL_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <locale>

#include <stdlib.h>
#include <string.h>

#include <string.h>
namespace Epic
{
    namespace Util
    {
        namespace detail
        {

            struct trim_front_and_back
            {
                std::string operator()(std::string & s)
                {
                    // trim whitespace from back and front
                    s.erase(
                            std::find_if(
                                s.rbegin(), s.rend(),
                                std::not1(std::ptr_fun<int, int>(std::isspace))
                                ).base(),
                            s.end());

                    s.erase(
                            s.begin(),
                            std::find_if(
                                s.begin(), s.end(),
                                std::not1(std::ptr_fun<int, int>(std::isspace))
                                )
                           );
                    return s;
                }
            };

            struct transpose_char_with_space_classification_facet: std::ctype<char>
            {
                transpose_char_with_space_classification_facet(int c): std::ctype<char>(get_table(c)) {}

                static std::ctype_base::mask const* get_table(int c)
                {
                    typedef std::ctype<char> cctype;

                     // cache a copy of the sane table
                    static const cctype::mask *const_rc= cctype::classic_table();

                    static cctype::mask rc[cctype::table_size];

                    ::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

                    // swap classification of delim with space
                    rc[' '] ^= rc[ c & 255u];
                    rc[c & 255u] ^= rc[' '];
                    rc[' '] ^= rc[c & 255u];

                    return &rc[0];
                }
            };
        } // detail

        template<typename T>
            void
            trimv(T begin, T end)
            {
                // trim tokens
                std::transform(
                        begin,end,
                        begin,
                        detail::trim_front_and_back()
                        );
            }

        template<typename T>
            T &
            split(T & dest, const std::string & s, const char *c)
            {
                std::stringstream ss(s);
                // TODO fix this
                ss.imbue(
                        std::locale(
                            std::locale(),
                            // I know it looks wrong but don't delete the pointer
                            // i think it gets deleted in ~ostringstream()
                            new detail::transpose_char_with_space_classification_facet((c)? *c : ' ')  // treat char as ' '
                            )
                        );

                std::istream_iterator<std::string>  begin(ss);
                std::istream_iterator<std::string>  end;

                dest.assign(begin,end);
                return dest;
            }



        // front
        inline std::string & rtrim(std::string & s) {
            s.erase(
                    s.begin(),
                    std::find_if(s.begin(), s.end(),
                        std::not1(std::ptr_fun<int, int>(
                                std::isspace
                                )
                            )
                        ) );
            return s;
        }

        // back
        inline std::string & ltrim(std::string & s) {
            s.erase(
                    std::find_if(s.rbegin(), s.rend(),
                        std::not1(std::ptr_fun<int, int>(
                                std::isspace
                                )
                            )
                        ).base(),
                    s.end()
                   );
            return s;
        }

        // front and back
        inline std::string & trim(std::string & s) {
            return ltrim(rtrim(s));
        }

        // return copyfront and back
        inline std::string c_trim(const std::string & s) {
            std::string cpy(s);
            return trim(cpy);
        }

    } // Epic::Util
} // Epic
#endif /*ndef EPIC_UTIL_HPP */
