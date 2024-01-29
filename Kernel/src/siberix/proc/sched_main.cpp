#include <siberix/proc/svchost.h>

namespace Kern::Svc {
    void TaskSvcHost::onLoad()
    {
        RefPtr<Process> kernelProcess =
          getProcessFactory()->createProcess("Kernel", ProcessType::Kernel);
        m_processes[0] = kernelProcess;
    }

}