#include <Network/ArkNet/ArkAddress.hpp>
#include <string.h>

#ifdef _WIN32
#include <ws2tcpip.h>
#endif

namespace ArkNet
{
    ArkAddress::ArkAddress()
    {
        memset(&address, 0, sizeof(address));
        address_size = sizeof(address);
    }

    std::string ArkAddress::toString()
    {
        std::string addr_str;
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &address.sin_addr, str, INET_ADDRSTRLEN);
        addr_str += str;
        addr_str += ":";
        addr_str += std::to_string(ntohs(address.sin_port));
        return addr_str;
    }
}