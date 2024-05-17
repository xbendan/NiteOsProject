#include <mixins/meta/field.h>
#include <mixins/types.h>

namespace sbrx::sched {
    using namespace mixins;

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
        enum class State
        {
            Running,
            Waiting,
            Sleeping,
            Terminated,
            Zombie,
        };

        /* --- Constructors --- */

        Thread() = delete;
        Thread(Process* process, uint32_t threadId);
        ~Thread() = default;

        /* --- Methods --- */

        Process* getParent() const { return m_process; }
        uint32_t getId() const { return m_threadId; }
        uint8_t  getPriority() { return m_priority; }
        State&   getState() { return m_state; }

        ReadOnly<Process*> parent;
        ReadWrite<State>   state;
        ReadOnly<uint32_t> id;
        ReadWrite<uint8_t> priority;

    private:
        Process* m_process;
        State    m_state;

        uint32_t m_threadId;
        uint8_t  m_priority;
    };
}