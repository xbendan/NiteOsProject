#include <Process/Process.h>

namespace Proc
{
    Process *GetKernelProcess();
    Process *GetCurrentProcess();

    pid_t GetNextPID();
    Process *GetProcessById(pid_t pid);
    
    void Initialize();
    void Schedule();
}