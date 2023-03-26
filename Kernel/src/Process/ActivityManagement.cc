#include <Proc/Activity.h>
#include <Proc/Scheduler.h>

namespace Task
{
    Activity g_SystemActivity("System", &g_KernelProcess);
} // namespace Task
