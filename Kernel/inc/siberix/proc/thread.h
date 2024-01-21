#pragma once

#include <stdcxx/types.h>
#include <xtra-concurrent/spinlock.h>

namespace Kern::Task {
    class Process;

    enum class ThreadState
    {
        Running,
        Waiting,
        Sleeping,
        Terminated,
        Zombie,
    };

    class Thread
    {
    public:
        Thread(Process* process, uint32_t threadId);
        ~Thread() = default;

        uint32_t getThreadId() const { return m_threadId; }
        Process* getParent() const { return m_process; }
        uint8_t  getPriority() { return m_priority; }

        ThreadState& state() { return m_state; }

    private:
        uint32_t    m_threadId;
        Process*    m_process;
        ThreadState m_state;
        uint8_t     m_priority;

        Spinlock m_lock;
        Spinlock m_stateLock;
    };
}