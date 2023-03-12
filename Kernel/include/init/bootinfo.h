#pragma once

#include <Mem/Memory.h>
#include <macros>

typedef struct BootInfoMemory
{
    uint64_t m_TotalSize;
    uint64_t m_Usable;
    size_t m_MemoryMapSize;
    Memory::MemoryMapEntry m_MemoryMapEntries[MEMORY_MAP_LIMIT];
} bootmem_t;

typedef struct BootInfoGraphics
{
    uint16_t m_Width, m_Height;
    uint64_t m_BufferAddress;
    uint16_t m_Pitch;
    uint16_t m_BytesPerPixel;
} bootgraphic_t;

struct BootInfo
{
    uint64_t m_Checksum;
    const char* m_BootloaderName;
    struct BootInfoMemory m_Memory;
    struct BootInfoGraphics m_Graphics;
};

extern BootInfo bootInfo;