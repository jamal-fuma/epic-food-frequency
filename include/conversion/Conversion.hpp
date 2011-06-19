#ifndef EPIC_CONVERSION_HPP
#define EPIC_CONVERSION_HPP

#include <cstdlib>

#include <string>
#include <map>

#include <ostream>

#include "config/Global.hpp"
#include "libhelper/Logger.hpp"

namespace Epic
{
    namespace Conversion
    {
        struct IntegerString
        {
            long m_num;
            bool m_valid;
            IntegerString(const std::string & s) : m_num(0),m_valid(true)
            {

                std::istringstream ss(s);
                ss >> m_num;
            }
            operator long() const
            {
                return m_num;
            }
        };

        struct DecimalString
        {
            double m_num;
            bool m_valid;
            DecimalString(const std::string & s) :m_num(-1.0F), m_valid(true)
            {
                if(s.find_first_of(".") == std::string::npos)
                {
                    IntegerString is(s);
                    long num = is;
                    m_num = (num *1.00F);
                }
                else 
                {
                    char * end=NULL;
                    double d =  std::strtod(s.c_str(),&end);
                    if(end == s)
                    {
                     //   Epic::Logging::Error().log() << "Turning [" << s << "] into a decimal number failed" << "\n";
                        m_valid = false;
                    }
                    else
                    {
                        m_valid = true;
                        m_num = d;
                    }
                }
            }

            operator double() const
            {
                return m_num ;
            }
        };

        /* this attempts to paper over the usage of a quantity as
         * a code */
        struct NutrientQuantity
        {
            double m_value;
            bool m_valid;

            NutrientQuantity(const std::string & s) :
                m_valid(false), m_value(-1.0F)
            {
                std::string s_val(s);

                // this is such a hack, but
                // the whole premise is sub-optimal
                // a quantity is supposed to be a quantity not a code
                //
                // Esentially if the value is an exact match in the config file we use it's
                // configured value otherwise we decode the literal symbol
                if(Epic::Config::Quantity::find(s,s_val))
                {
                    // delegate the conversion proper ie. checking it's actually some
                    // sort of number to DecimalString's implementation
                    DecimalString ds(s_val);
                    m_value = ds;
                    m_valid = ds.m_valid;
                }
                else
                {
                    // delegate the conversion proper ie. checking it's actually some
                    // sort of number to DecimalString's implementation
                    DecimalString ds(s_val);
                    m_value = ds;
                    m_valid = ds.m_valid;
                }
            }

            operator double() const
            {
                return m_value;
            }
        };

    } // Epic::Conversion
} // Epic

#endif /* EPIC_CONVERSION_HPP */


