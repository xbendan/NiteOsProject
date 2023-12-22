#include <stdcxx/types.h>

static inline Byte
inByte8(uint16_t port)
{
    uint8_t data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline Word
inWord16(uint16_t port)
{
    uint16_t data;
    asm volatile("inw %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline DWord
inDWord32(uint16_t port)
{
    uint32_t data;
    asm volatile("inl %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline void
outByte8(uint16_t port, Byte data)
{
    asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

static inline void
outWord16(uint16_t port, Word data)
{
    asm volatile("outw %0, %1" : : "a"(data), "d"(port));
}

static inline void
outDWord32(uint16_t port, DWord data)
{
    asm volatile("outl %0, %1" : : "a"(data), "d"(port));
}

static inline void
waitIO()
{
    outByte8(0x80, 0);
}