#pragma once

namespace ArkNet
{
    struct ArkPacket
    {
        ArkPacket* owner;
        char* data;
        int dataSz;

        ArkPacket();
        ArkPacket(int sz);
        ~ArkPacket();

        ArkPacket sub(int start, int sz);
    };

}