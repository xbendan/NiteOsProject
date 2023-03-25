#include <Proc/Process.h>
#include <Proc/Scheduler.h>
#include <Proc/Activity.h>
#include <Mem/KMemAlloc.h>
#include <libkern/objects.h>
#include <Utils/ArrayList.h>

#if defined(ARCH_X86_64)

#include <Arch/x86_64/CPU.h>

#endif

namespace Task
{
    Process g_KernelProcess(
        "atrikrnl",
        nullptr,
        0,
        &g_SystemActivity,
        TaskType::TaskTypeSystemProcess);
    Scheduler g_Scheduler = Scheduler();

    Process* GetCurrentProcess() {
        Thread *currentThread = GetCPULocal()->currentThread;

        return Objects::IsNull(currentThread) ?
                nullptr :
                currentThread->m_Parent;
    }
}