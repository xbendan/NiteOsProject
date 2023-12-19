#include <stdcxx/types.h>

namespace Xtra::Network {
    struct IPv4Address
    {
        union
        {
            UInt8  _bytes[4];
            UInt16 _words[2];
            UInt32 _dwords[1];
        };
    };

    struct IPv6Address
    {
        union
        {
            UInt8  _bytes[6];
            UInt16 _words[3];
        };
    };
}