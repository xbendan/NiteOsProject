static inline uint8_t inByte8(uint16_t port) {
    uint8_t data;
    asm volatile("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline uint16_t inWord16(uint16_t port) {
    uint16_t data;
    asm volatile("inw %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline uint32_t inDWord32(uint16_t port) {
    uint32_t data;
    asm volatile("inl %1, %0" : "=a"(data) : "d"(port));
    return data;
}

static inline void outByte8(uint16_t port, uint8_t data) {
    asm volatile("outb %0, %1" : : "a"(data), "d"(port));
}

static inline void outWord16(uint16_t port, uint16_t data) {
    asm volatile("outw %0, %1" : : "a"(data), "d"(port));
}

static inline void outDWord32(uint16_t port, uint32_t data) {
    asm volatile("outl %0, %1" : : "a"(data), "d"(port));
}

static inline void waitIO() { outByte8(0x80, 0); }