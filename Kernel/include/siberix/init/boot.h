#pragma once

#include <common/typedefs.h>
#include <siberix/mm/types.h>

#ifdef ARCH_X86_64
#define BUILD_ARCH "x86_64"
#elif ARCH_RISCV
#define BUILD_ARCH "riscv"
#endif

enum BootloaderName
{
    BL_LIMINE,
    BL_NBW
};

struct BootConfig
{
    u64   checksum;
    u64   blTime;
    char* blName;
    struct BootMemory
    {
        u64       totalSize;
        u64       maxSize;
        u64       usableSize;
        PageBlock ranges[64];
    } memory;
    struct BootGraphics
    {
        u64 width;
        u64 height;
        u64 address;
        u64 pitch;
        u16 bpp;
    } graphic[16];
};

void
kmain(BootConfig& bootConfig);