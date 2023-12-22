#include <siberix/proc/thread.h>
#include <stdcxx/types.h>

namespace Kern::Hal {
    struct Cpu
    {
        Cpu*                m_self;
        uint32_t              m_processorId;
        Kern::Task::Thread* m_currentThread;
        Kern::Task::Thread* m_idleThread;

        constexpr Cpu(uint32_t processorId)
          : m_self(this)
          , m_processorId(processorId)
          , m_currentThread(nullptr)
          , m_idleThread(nullptr)
        {
        }
    };
}