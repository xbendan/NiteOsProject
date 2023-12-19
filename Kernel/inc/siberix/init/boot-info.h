#include <stdcxx/range.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Kern::Init {
    struct BootConfigTable
    {
        UInt64 _checksum;
        UInt64 _bootTime;
        Char*  _bootloaderName;

        UInt64             _memTotalSize;
        UInt64             _memMaxSize;
        UInt64             _memUsableSize;
        Std::Range<UInt64> _memRanges;

        UInt64 _gfxWidth;
        UInt64 _gfxHeight;
        UInt64 _gfxAddress;
        UInt64 _gfxPitch;
        UInt16 _gfxBpp;
    };
}