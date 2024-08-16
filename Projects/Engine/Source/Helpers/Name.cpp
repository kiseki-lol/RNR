#include <Helpers/Name.hpp>

namespace RNR
{
    std::map<int, RNR::Name*>* Name::dictionary = new std::map<int, RNR::Name*>();

    char* Name::c_str()
    {
        return NULL;
    }

    void Name::compare(const RNR::Name* a, const RNR::Name* b)
    {
        //
    }

    void Name::declare(const char* sName, int dictionaryIndex)
    {
        //
    }
}