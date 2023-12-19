#include <siberix/proc/thread.h>
#include <stdcxx/types.h>

namespace Kern::Hal {
    struct Cpu
    {
        Cpu*                m_self;
        UInt32              m_processorId;
        Kern::Task::Thread* m_currentThread;
        Kern::Task::Thread* m_idleThread;
    };
}