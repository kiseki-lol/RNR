#pragma once

#include <map>
#include <cstddef>

namespace RNR
{
    class Name
    {
        public:
            char* c_str();

            static void compare(const RNR::Name* a, const RNR::Name* b);
            static void declare(const char* sName, int dictionaryIndex);
            static std::map<int, RNR::Name*>* dictionary;
    };
}