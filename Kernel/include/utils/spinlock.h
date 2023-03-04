#pragma once

inline void AcquireLock(volatile int *lock)
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

    inline void ReleaseLock(volatile int *lock)
    {
        int unlockVal = 0;
        __asm__ volatile(
                "xchg %[value], %[ulv]\n"
                : [value] "=m" (*lock), [ulv] "=q" (unlockVal)
                : "[ulv]" (unlockVal)
                : "memory");
    }

class ScopedSpinlock final {
        volatile int m_Lock;
        ScopedSpinlock(volatile int& lock) : m_Lock(lock) { AcquireLock(&m_Lock); }
        ~ScopedSpinlock() { ReleaseLock(&m_Lock); }
    };

typedef class Spinlock
    {
        volatile int m_Lock;
    public:
        void Acquire() { AcquireLock(&m_Lock); }
        void Release() { ReleaseLock(&m_Lock); }
    } spinlock_t;