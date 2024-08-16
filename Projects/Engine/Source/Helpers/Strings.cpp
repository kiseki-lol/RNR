#include <Helpers/Strings.hpp>

namespace RNR
{
    uint8_t Strings::random_char()
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 255);
        return static_cast<uint8_t>(dis(gen));
    }

    std::string Strings::random_hex(const uint64_t length)
    {
        std::stringstream result;

        for (uint64_t i = 0; i < length; i++)
        {
            uint8_t rc = random_char();
            std::stringstream hexstream;
            hexstream << std::hex << int(rc);
            std::string hex = hexstream.str();
            result << (hex.length() < 2 ? '0' + hex : hex);        
        }

        return result.str();
    }
}