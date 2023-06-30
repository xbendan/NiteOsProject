#pragma once

#include <stdint.h>

namespace Ports
{
    static inline uint8_t ReadByte8(uint16_t port)
    {
        uint8_t data;
        asm volatile("inb %1, %0"
                    : "=a"(data)
                    : "d"(port));
        return data;
    }

    static inline uint16_t ReadWord16(uint16_t port)
    {
        uint16_t data;
        asm volatile("inw %1, %0"
                    : "=a"(data)
                    : "d"(port));
        return data;
    }

    static inline uint32_t ReadDword32(uint16_t port)
    {
        uint32_t data;
        asm volatile("inl %1, %0"
                    : "=a"(data)
                    : "d"(port));
        return data;
    }

    static inline void WriteByte8(uint16_t port, uint8_t data)
    {
        asm volatile("outb %0, %1"
                    :
                    : "a"(data), "d"(port));
    }

    static inline void WriteWord16(uint16_t port, uint16_t data)
    {
        asm volatile("outw %0, %1"
                    :
                    : "a"(data), "d"(port));
    }

    static inline void WriteDword32(uint16_t port, uint32_t data)
    {
        asm volatile("outl %0, %1"
                    :
                    : "a"(data), "d"(port));
    }

    static inline void IoWait() 
    {
        WriteByte8(0x80, 0);
    }
} // namespace Ports