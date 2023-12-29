#include <siberix/proc/thread.h>
#include <stdcxx/types.h>

namespace Kern::Hal {
    class CPU
    {
    public:
        enum class Signal : uint32_t
        {
            INIT  = 0,
            START = 1,
            HALT  = 2,
            RESET = 3,
            SCHED = 4,
        };

        CPU() = delete;
        constexpr CPU(uint32_t processorId)
          : m_self(this)
          , m_processorId(processorId)
          , m_currentThread(nullptr)
          , m_idleThread(nullptr)
        {
        }

        void sendSignal(Signal signal);

        inline void enableInterrupts();
        inline void disableInterrupts();

    private:
        CPU*                m_self;
        uint32_t            m_processorId;
        Kern::Task::Thread* m_currentThread;
        Kern::Task::Thread* m_idleThread;
    };

    class ICpuHost
    {
    public:
        virtual ~ICpuHost() = default;

        virtual uint32_t getCpuAmount()               = 0;
        virtual CPU*     getCpu(uint32_t processorId) = 0;
        virtual CPU*     getCpuCurrent()              = 0;
        virtual void     launchAll()                  = 0;
    };

    class IPowerManagement
    {
    public:
        virtual ~IPowerManagement() = default;

        virtual void shutdown() = 0;
        virtual void sleep()    = 0;
        virtual void wake()     = 0;
    };
}