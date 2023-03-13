#pragma once

// inline void AcquireLock(volatile int *lock)
// {
//     int lockVal = 1; // From DPDK
//     __asm__ volatile(
//         "1:\n"
//         "xchg %[value], %[lv]\n"
//         "test %[lv], %[lv]\n"
//         "jz 3f\n"
//         "2:\n"
//         "pause\n"
//         "cmpl $0, %[value]\n"
//         "jnz 2b\n"
//         "jmp 1b\n"
//         "3:\n"
//         : [value] "=m"(*lock), [lv] "=q"(lockVal)
//         : "[lv]"(lockVal)
//         : "memory");
// }

// inline void ReleaseLock(volatile int *lock)
// {
//     int unlockVal = 0;
//     __asm__ volatile(
//         "xchg %[value], %[ulv]\n"
//         : [value] "=m"(*lock), [ulv] "=q"(unlockVal)
//         : "[ulv]"(unlockVal)
//         : "memory");
// }

#define acquireLock(lock)                                                                                              \
    ({                                                                                                                 \
        while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE))                                                         \
            asm("pause");                                                                                              \
    })

#define acquireLockIntDisable(lock)                                                                                    \
    ({                                                                                                                 \
        asm volatile("cli");                                                                                           \
        while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE))                                                         \
            asm volatile("sti; pause; cli");                                                                           \
    })

#define releaseLock(lock) ({ __atomic_store_n(lock, 0, __ATOMIC_RELEASE); });

class ScopedSpinlock final
{
    volatile int m_Lock;
    ScopedSpinlock(volatile int &lock) : m_Lock(lock) { acquireLock(&m_Lock); }
    ~ScopedSpinlock() { releaseLock(&m_Lock); }
};

typedef class Spinlock
{
    volatile int m_Lock;

public:
    void Acquire() { acquireLock(&m_Lock); }
    void Release() { releaseLock(&m_Lock); }
} spinlock_t;