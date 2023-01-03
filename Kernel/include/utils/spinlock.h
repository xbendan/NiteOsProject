#pragma once

namespace Utils
{
    typedef volatile int spinlock_t;

    inline void AcquireLock(spinlock_t *lock)
    {
        int lockVal = 1; // From DPDK
        __asm__ volatile(
                "1:\n"
                "xchg %[value], %[lv]\n"
                "test %[lv], %[lv]\n"
                "jz 3f\n"
                "2:\n"
                "pause\n"
                "cmpl $0, %[value]\n"
                "jnz 2b\n"
                "jmp 1b\n"
                "3:\n"
                : [value] "=m" (*lock), [lv] "=q" (lockVal)
                : "[lv]" (lockVal)
                : "memory");
    }

    inline void ReleaseLock(spinlock_t *lock)
    {
        int unlockVal = 0;
        __asm__ volatile(
                "xchg %[value], %[ulv]\n"
                : [value] "=m" (*lock), [ulv] "=q" (unlockVal)
                : "[ulv]" (unlockVal)
                : "memory");
    }

    class ScopedSpinlock
    {
        spinlock_t m_Lock;
        ScopedSpinlock(spinlock_t& lock) : m_Lock(lock) { AcquireLock(&m_Lock); }
        ~ScopedSpinlock() { ReleaseLock(&m_Lock); }
    };

    class Spinlock
    {
        spinlock_t m_Lock;
    public:
        void Acquire() { AcquireLock(&m_Lock); }
        void Release() { ReleaseLock(&m_Lock); }
    };
}