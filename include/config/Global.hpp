#ifndef EPIC_CONFIG_GLOBAL_HPP
#define EPIC_CONFIG_GLOBAL_HPP

#include "pattern/Singleton.hpp"
#include "config/Reader.hpp"
#include <stdexcept>
#include <sstream>

namespace Epic
{
    namespace Config
    {
      class Config
      {
            std::map<std::string,std::string> m_params;

            public:
                Config() {}

                bool find(const std::string & key, std::string & dest);

                void load(const std::string & filename) ;

        };

        bool load(const std::string & filename);

        bool find(const std::string & key, std::string & dest);


        namespace Field
        {
            class FieldConfig : public Config
            {
                public:
                    FieldConfig(){}
            };

            bool load(const std::string & filename);

            bool find(const std::string & key, std::string & dest);

        } // Epic::Config::Field

        namespace Quantity
        {
            class QuantityConfig : public Config
            {
                public:
                    QuantityConfig (){}
            };

            bool load();

            bool find(const std::string & key, std::string & dest);
        } // Epic::Config::Quantity

    } // Epic::Config
} // Epic
#endif /*ndef EPIC_CONFIG_GLOBAL_HPP */

