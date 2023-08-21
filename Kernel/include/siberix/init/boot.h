#pragma once

#include <common/typedefs.h>
#include <siberix/mm/types.h>

#ifdef ARCH_X86_64
#define BUILD_ARCH "x86_64"
#elif ARCH_RISCV
#define BUILD_ARCH "riscv"
#endif

enum BootloaderName {
    BL_LIMINE,
    BL_NBW
};

struct BootConfig {
    u64            checksum;
    BootloaderName blName;
    struct BootMemory {
        u64       totalSize;
        u64       maxSize;
        PageBlock ranges[64];
    } memory;
    struct BootGraphics {
        u16 width;
        u16 height;
        u64 address;
        u16 pitch;
        u16 bpp;
    } graphic;
};

void kmain(BootConfig& bootConfig);