#ifndef EPIC_IMPORT_HPP
#define EPIC_IMPORT_HPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include "../include/dataset/Dataset.hpp"
#include "../include/import/Parser.hpp"
#include "../include/import/Context.hpp"

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
   
    } // Epic::Import
} // Epic

#endif /* EPIC_IMPORT_HPP */


