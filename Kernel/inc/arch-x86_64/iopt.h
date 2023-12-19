#include <stdcxx/types.h>

static inline Byte
inByte8(UInt16 port)
{
    UInt8 data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline Word
inWord16(UInt16 port)
{
    UInt16 data;
    asm volatile("inw %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline DWord
inDWord32(UInt16 port)
{
    UInt32 data;
    asm volatile("inl %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline void
outByte8(UInt16 port, Byte data)
{
    asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

static inline void
outWord16(UInt16 port, Word data)
{
    asm volatile("outw %0, %1" : : "a"(data), "d"(port));
}

static inline void
outDWord32(UInt16 port, DWord data)
{
    asm volatile("outl %0, %1" : : "a"(data), "d"(port));
}

static inline void
waitIO()
{
    outByte8(0x80, 0);
}