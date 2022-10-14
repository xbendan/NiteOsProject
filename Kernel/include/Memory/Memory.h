#pragma once

#include <macros>
#include <init/bootinfo>
#define ARCH_PAGE_SIZE (4096)

extern uint64_t KERNEL_START_ADDR;
extern uint64_t KERNEL_END_ADDR;

void* memset(void* addr, int val, size_t count);
void* memcpy(void* dest, const void* addr, size_t count);
int memcmp(const void* s1, const void* s2, size_t n);

enum mem_stats_item
{
    MemValueTotal = 0,
    MemValueAvaliable = 1,
    MemValuePresent = 2,
    MemValueSwapped = 3,
    MemValueInused = 4,
    MemValueStandby = 5,
    MemValueCached = 6,
    MemValueAllocated = 7
};

struct mem_stats
{
    uint64_t total;
    uint64_t available;
    uint64_t present;
    uint64_t swapped;
    uint64_t inuse;
    uint64_t standby;
    uint64_t cached;
    uint64_t allocated;
};

void MemoryInitialize();

//void mem_value_commit(enum mem_stats_item item_num, int64_t change);