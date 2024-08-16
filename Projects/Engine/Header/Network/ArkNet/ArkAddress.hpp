#pragma once

#include <Network/ArkNet/PlatformNeutralSockets.hpp>

#include <string>

namespace ArkNet
{
    struct ArkAddress
    {
        struct sockaddr_in address;
        size_t address_size;
        
        ArkAddress();
        std::string toString();
    };
}