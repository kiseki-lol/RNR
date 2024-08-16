#pragma once
#include <map>
#include <string>
#include <Network/ArkNet/ArkStream.hpp>

namespace ArkNet
{
    union ArkUserDataValue
    {
        int integer;
        float number;
    };

    enum ArkUserDataType
    {
        ARKUSERDATA_STRING, 
        ARKUSERDATA_INTEGER, 
        ARKUSERDATA_NUMBER
    };

    struct ArkUserDataEntry
    {
        ArkUserDataType type;
        bool replicate;
        std::string string;
        ArkUserDataValue value;
    };

    class ArkUserData
    {
    private:
        std::map<std::string, ArkUserDataEntry> m_userProperties;
    public:
        ArkUserData();

        void writeToStream(ArkStream* stream);
        void readFromStream(ArkStream* stream);

        ArkUserDataEntry& getEntry(std::string key);
        void setEntry(std::string key, ArkUserDataEntry entry);
    };
}