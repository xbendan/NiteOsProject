#include <siberix/proc/sched.h>

namespace Kern::Task {
    void TaskSvcHost::onLoad()
    {
        m_processFactory = new ProcessFactory(this);
        m_scheduler      = new Scheduler();
        m_processes      = Std::Array<RefPtr<Process>>(256);
    }
}