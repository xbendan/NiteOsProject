#include <arch/x86_64/types.h>
#include <siberix/proc/types.h>

struct X64Thread : public Thread {
    struct {
        u32 esp0;
        u32 ss0;
    };

    u64   fsBase;
    void* fxState;

    void* m_userStack;
    void* m_userStackBase;
    void* m_kernelStack;
    void* m_kernelStackBase;

    RegisterContext m_registers;
    RegisterContext m_lastSyscall;
};