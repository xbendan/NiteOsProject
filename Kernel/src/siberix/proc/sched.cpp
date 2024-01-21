#include <siberix/proc/svchost.h>

namespace Kern::Svc {
    void TaskSvcHost::onLoad()
    {
        RefPtr<Process> kernelProcess =
          getProcessFactory()->createProcess("Kernel", 0, );
    }
}