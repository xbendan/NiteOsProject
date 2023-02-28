#pragma once

#include <Arch/x86_64/cpu.h>

void DisableInterrupts();
void EnableInterrupts();

typedef void (*isr_t)(void*, registers_t*);

void RegisterInterruptHandler(int num, isr_t func);

static inline bool IsInterruptsEnabled() 
{
    uint64_t flags;
    asm volatile ( "pushf\n\t"
                   "pop %0"
                   : "=g"(flags) );
    return flags & (1 << 9);
}

class InterruptsRetainer
{
public:
    InterruptsRetainer() { asm("cli"); }
    ~InterruptsRetainer() { asm("sti"); }
};