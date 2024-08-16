#include <Network/ArkNet/ArkUserData.hpp>

namespace ArkNet
{
    ArkUserData::ArkUserData()
    {
    }

    void ArkUserData::writeToStream(ArkStream *stream)
    {
        stream->write<unsigned short>(m_userProperties.size());
        for (auto &[key, entry] : m_userProperties)
        {
            if (!entry.replicate)
                continue;
            stream->write<ArkUserDataType>(entry.type);
            stream->writeString(key);
            switch (entry.type)
            {
            case ARKUSERDATA_STRING:
                stream->writeString(entry.string);
                break;
            case ARKUSERDATA_INTEGER:
                stream->write(entry.value.integer);
                break;
            case ARKUSERDATA_NUMBER:
                stream->write(entry.value.number);
                break;
            }
        }
    }

    void ArkUserData::readFromStream(ArkStream *stream)
    {
        int prop_read = stream->read<unsigned short>();
        for (int i = 0; i < prop_read; i++)
        {
            ArkUserDataType type = stream->read<ArkUserDataType>();
            ArkUserDataEntry entry;
            entry.type = type;
            entry.replicate = true;
            std::string prop_name = stream->readString();
            switch (type)
            {
            case ARKUSERDATA_STRING:
                entry.string = stream->readString();
                break;
            case ARKUSERDATA_INTEGER:
                entry.value.integer = stream->read<int>();
                break;
            case ARKUSERDATA_NUMBER:
                entry.value.number = stream->read<float>();
                break;
            }
            setEntry(prop_name, entry);
        }
    }
}