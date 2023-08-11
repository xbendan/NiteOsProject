#include <common/typedefs.h>

static inline u8 inByte8(u16 port) {
    u8 data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline u16 inWord16(u16 port) {
    u16 data;
    asm volatile("inw %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline u32 inDWord32(u16 port) {
    u32 data;
    asm volatile("inl %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline void outByte8(u16 port, u8 data) {
    asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

static inline void outWord16(u16 port, u16 data) {
    asm volatile("outw %0, %1" : : "a"(data), "d"(port));
}

static inline void outDWord32(u16 port, u32 data) {
    asm volatile("outl %0, %1" : : "a"(data), "d"(port));
}

static inline void waitIO() { outByte8(0x80, 0); }