#include <stdcxx/types.h>

namespace Xtra::Network {
    struct IPv4Address
    {
        union
        {
            uint8_t  _bytes[4];
            uint16_t _words[2];
            uint32_t _dwords[1];
        };
    };

    struct IPv6Address
    {
        union
        {
            uint8_t  _bytes[6];
            uint16_t _words[3];
        };
    };
}