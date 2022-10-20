#pragma once

#include <Memory/Memory.h>
#include <Utils/Range.h>
#include <Macros>

struct BootInfoMemory
{
    uint64_t m_TotalSize;
    uint64_t m_Usable;
    size_t m_MemoryMapSize;
    Memory::MemoryMapEntry m_MemoryMapEntries[MEMORY_MAP_LIMIT];
};

struct BootInfoGraphics
{
    uint16_t m_Width, m_Height;
    uint64_t m_BufferAddress;
    uint16_t m_Pitch;
    uint16_t m_BytesPerPixel;
};

struct BootInfo
{
    uint64_t m_Checksum;
    const char* m_BootloaderName;
    struct BootInfoMemory m_Memory;
    struct BootInfoGraphics m_Graphics;
};

BootInfo* GetBootInfo();