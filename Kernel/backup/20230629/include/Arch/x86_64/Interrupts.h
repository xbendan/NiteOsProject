#pragma once

#include <Arch/x86_64/CPU.h>

inline void DisableInterrupts() { asm("cli"); }
inline void EnableInterrupts() { asm("sti"); }

struct Interrupt;

typedef void (*irqhandle_t)(Interrupt*, RegisterContext*);

enum InterruptType
{
    IntTypeFault = 0x1,
    IntTypeTrap = 0x2,
    IntTypeInterrupt = 0x4,
    IntTypeAbort = 0x8
};

struct Interrupt
{
    const char *m_name;
    uint8_t type;
    bool m_hasErrCode;
    irqhandle_t handler;

    bool IsFault() { return (type & IntTypeFault); }
    bool IsTrap() { return (type & IntTypeTrap); }
    bool IsInterrupt() { return (type & IntTypeInterrupt); }
    bool HasErrorCode() { return m_hasErrCode; }
};

bool RegisterIRQ(uint8_t intno, irqhandle_t handler);

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