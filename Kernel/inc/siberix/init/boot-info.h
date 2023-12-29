#pragma once

#include <stdcxx/range.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>

#define BOOT_MEMMAP_USABLE 1
#define BOOT_MEMMAP_KERNEL_AND_MODULES 2
#define BOOT_MEMMAP_ACPI_RECLAIMABLE 3
#define BOOT_MEMMAP_ACPI_NVS 4
#define BOOT_MEMMAP_BAD_MEMORY 5
#define BOOT_MEMMAP_BOOTLOADER_RECLAIMABLE 6
#define BOOT_MEMMAP_RESERVED 7

namespace Kern::Init {
    struct BootConfigTable
    {
        uint64_t _checksum;
        uint64_t _bootTime;

        struct Mem
        {
            uint64_t _totalSize;
            uint64_t _maxSize;
            uint64_t _usableSize;
            struct Memmap
            {
                Std::Range<uint64_t> _value;
                uint8_t              _type;

                constexpr Memmap() = default;

                constexpr Memmap(uint64_t start, uint64_t end, uint8_t type)
                  : _value(start, end)
                  , _type(type)
                {
                }
            } _ranges[32];
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