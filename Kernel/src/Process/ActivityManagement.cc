#include <proc/activity.h>
#include <proc/sched.h>

namespace Task
{
    Activity g_SystemActivity(
        "System",
        &g_KernelProcess
    );
} // namespace Task
