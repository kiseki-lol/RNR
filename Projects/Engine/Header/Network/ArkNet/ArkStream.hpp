#pragma once
#include <Network/ArkNet/ArkPacket.hpp>
#include <stdexcept>
#include <string.h>
#include <string>

namespace ArkNet
{
    class ArkStream
    {
        char* m_data;
        int m_dataOff;
        int m_dataSize;
    public:
        ArkStream(char* data, int data_size);
        ArkStream(ArkPacket* packet);

        template<typename T>
        void write(T val)
        {
            size_t val_size = sizeof(val);
            if((m_dataOff + val_size) > m_dataSize)
                throw std::runtime_error("Overflow");
            memcpy(m_data + m_dataOff, &val, val_size);
            m_dataOff += val_size;            
        }

        template<typename T>
        T read()
        {
            size_t val_size = sizeof(T);
            if((m_dataOff - val_size) < 0)
                throw std::runtime_error("Underflow");
            T* ref = (T*)(m_data + m_dataOff);
            m_dataOff += val_size;
            return *ref;
        }

        ArkPacket readPacket(int sz);

        // # of bytes that have been written to/read from the stream
        int size() { return m_dataOff; } 

        void writeString(std::string val);
        std::string readString();
    };
}