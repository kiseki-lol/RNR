#include <Network/ArkNet/ArkPacket.hpp>
#include <stdio.h>

namespace ArkNet
{
    ArkPacket::ArkPacket()
    {
        owner = NULL;
        data = NULL;
        dataSz = 0;
    }

    ArkPacket::ArkPacket(int sz)
    {
        owner = NULL;
        data = new char[sz];
        dataSz = sz;
    }

    ArkPacket::~ArkPacket()
    {
        if(!owner && data)
        {
            delete[] data;
            data = NULL;
        }
    }

    ArkPacket ArkPacket::sub(int start, int sz)
    {
        if(start + sz > dataSz)
            printf("ArkPacket::sub: warning: subpacket is larger then parent packet\n");
        ArkPacket p;
        p.data = data + start;
        p.dataSz = sz;
        p.owner = this;
        return p;
    }
}