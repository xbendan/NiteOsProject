#pragma once

#include <mixins/concurrent/atomic.h>
#include <mixins/types.h>

#include <stdcxx/types.h>
#include <xtra-concurrent/atomic.h>

class Spinlock
{
public:
    Spinlock()
      : m_lock(Atomic<bool>(false))
    {
    }
    ~Spinlock() = default;

    bool acquireTest()
    {
        bool result = m_lock.compareAndExchange(false, true);
        // Memory barrier

        if (!result) {
            // leave interrupt disabled
        }
        return result;
    }

    void acquire()
    {
        while (m_lock.exchange(true, AtomicOpcode::Acquired))
            asm volatile("pause");
    }

    void release()
    {
        asm volatile("cli");
        while (m_lock.exchange(false, AtomicOpcode::Acquired))
            asm volatile("sti; pause; cli");
    }

private:
    Atomic<bool> m_lock;
};

class ScopedSpinlock final
{
public:
    ScopedSpinlock(Spinlock& lock)
      : m_lock(lock)
    {
        lock.acquire();
    }
    ~ScopedSpinlock() { m_lock.release(); }

private:
    Spinlock& m_lock;
};