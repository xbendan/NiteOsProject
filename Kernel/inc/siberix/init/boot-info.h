#include <stdcxx/range.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Kern::Init {
    struct BootConfigTable
    {
        uint64_t _checksum;
        uint64_t _bootTime;

        struct Mem
        {
            uint64_t             _totalSize;
            uint64_t             _maxSize;
            uint64_t             _usableSize;
            Std::Range<uint64_t> _ranges[32];
        } _mem;

        struct Gfx
        {
            uint64_t _width;
            uint64_t _height;
            uint64_t _address;
            uint64_t _pitch;
            uint16_t _bpp;
        } _gfx;
    };
}