#include <macros>

struct ProcessorUsage
{
    uint64_t speed;
    uint64_t baseSpeed;
    uint16_t sockets;
    uint16_t physCores;
    uint16_t logicalCores;
    uint32_t cacheL1;
    uint32_t cacheL2;
    uint32_t cacheL3;
};

struct MemoryUsage
{
    uint64_t used;
    uint64_t free;
    uint64_t committed;
    uint64_t cached;
    uint64_t speed;
    uint8_t sockets;
    uint64_t reserved;
};

struct DiskUsage {
    char *name; /* The name of disk in hardware */
    char driverLetter;
    uint64_t capacity;
    uint64_t formatted;
};

struct NetworkUsage {
    char *networkName;
    char *adapterName;
    uint8_t[4] IPv4;
    uint8_t[6] IPv6;
    uint8_t connectionType;

    uint64_t linkSpeed;
    uint8_t state;
    uint64_t bytesSent;
    uint64_t bytesReceived;
};

struct GraphUsage {
    char *name;
    uint16_t[4] driverVersion;
    MemoryUsage gpuMemory;
}

struct SystemUsageProfile
{
    ProcessorUsage cpu;
    MemoryUsage memory;
    DiskUsage *disks;
    NetworkUsage *networks;
    GraphUsage *gpu;
};