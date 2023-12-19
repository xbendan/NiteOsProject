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
        Thread(Process* process, UInt32 threadId);
        ~Thread() = default;

        UInt32   getThreadId() const { return m_threadId; }
        Process* getParent() const { return m_process; }
        UInt8    getPriority() { return m_priority; }

        ThreadState& state() { return m_state; }

    private:
        UInt32      m_threadId;
        Process*    m_process;
        ThreadState m_state;
        UInt8       m_priority;

        Xtra::Concurrent::Spinlock m_lock;
        Xtra::Concurrent::Spinlock m_stateLock;
    };
}