#include <stdcxx/range.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Kern::Init {
    struct BootConfigTable
    {
        uint64_t _checksum;
        uint64_t _bootTime;
        Char*  _bootloaderName;

        uint64_t             _memTotalSize;
        uint64_t             _memMaxSize;
        uint64_t             _memUsableSize;
        Std::Range<uint64_t> _memRanges;

        uint64_t _gfxWidth;
        uint64_t _gfxHeight;
        uint64_t _gfxAddress;
        uint64_t _gfxPitch;
        uint16_t _gfxBpp;
    };
}