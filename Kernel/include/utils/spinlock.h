#pragma once

#define acquireLock(lock)                                                    \
    ({                                                                       \
        while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE)) asm("pause"); \
    })

#define acquireLockIntDisable(lock)                            \
    ({                                                         \
        asm volatile("cli");                                   \
        while (__atomic_exchange_n(lock, 1, __ATOMIC_ACQUIRE)) \
            asm volatile("sti; pause; cli");                   \
    })

#define releaseLock(lock) ({ __atomic_store_n(lock, 0, __ATOMIC_RELEASE); });

typedef volatile int spinlock_t;

class ScopedSpinlock final {
    volatile int m_Lock;
    ScopedSpinlock(volatile int &lock)
        : m_Lock(lock) {
        acquireLock(&m_Lock);
    }
    ~ScopedSpinlock() { releaseLock(&m_Lock); }
};

class Spinlock {
public:
    volatile int m_Lock;
    void         acquire() { acquireLock(&m_Lock); }
    void         release() { releaseLock(&m_Lock); }
};