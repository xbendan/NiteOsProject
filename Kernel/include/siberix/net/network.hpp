#include <common/string.h>
#include <common/typedefs.h>

namespace Network {
    struct IPv4Address {
        union {
            u32 data32;
            u8  data8s[4];
        };

        IPv4Address(u32 data)
            : data32(data) {}

        IPv4Address(u8 s1, u8 s2, u8 s3, u8 s4) {
            data8s[0] = s1;
            data8s[1] = s2;
            data8s[2] = s3;
            data8s[3] = s4;
        }

        IPv4Address& operator=(const u8 newData[]) {
            memcpy(data8s, newData, 4);
            return *this;
        }

        IPv4Address& operator=(const u32 newData) {
            *((u32*)data) = newData;
            return *this;
        }
    };

    struct IPv6Address {
        u8 data[6];
    };

    struct MacAddress {
        u8 data[6];
    };
}  // namespace Network