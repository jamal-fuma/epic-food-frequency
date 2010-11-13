#ifndef EPIC_CONVERSION_HPP
#define EPIC_CONVERSION_HPP

#include "Epic.hpp"

#include <string>
#include <map>
#include "config/Global.hpp"

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
                const char *c_str = s.c_str();
                if(-1 == ::utility_sntol(&m_num,c_str,strlen(c_str)))
                    m_valid = false;
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
            DecimalString(const std::string & s) :m_num(0.0F), m_valid(true)
            {
                if(s.find_first_of(".") == std::string::npos)
                {
                    IntegerString is(s);
                    long num = is;
                    m_num = (num * 100.00F);
                }
                // turns 0.10 into 10 which is perfect for financial calculations
                // but sub-optimal for other applications
                else if(-1 == ::utility_strtod(&m_num,s.c_str()))
                {
                    m_valid = false;
                }
            }

            operator double() const
            {
                if(m_valid)
                    return m_num / 100.00F;

                return 0.0F;
            }
        };

        /* this attempts to paper over the usage of a quantity as
         * a code */
        struct NutrientQuantity
        {
            double m_value;
            bool m_valid;

            NutrientQuantity(const std::string & s) :
                m_valid(false)
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


