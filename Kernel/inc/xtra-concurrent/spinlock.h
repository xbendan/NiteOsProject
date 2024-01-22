#pragma once

#include <stdcxx/types.h>
#include <xtra-concurrent/atomic.h>

class Spinlock
{
public:
    Spinlock()
      : m_lock(false)
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

    bool acquire()
    {
        while (!acquireTest()) {
            // wait
        }
    }

    void release()
    {
        // Memory barrier
        m_lock.store(false);
        // Leave interrupt disabled
    }

private:
    Atomic<bool> m_lock;
};