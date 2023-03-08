#include <proc/proc.h>
#include <proc/sched.h>
#include <proc/activity.h>
#include <mm/kmalloc.h>
#include <libkern/objects.h>
#include <utils/ArrayList.h>

#if defined(ARCH_X86_64)

#include <Arch/x86_64/cpu.h>

#endif

namespace Task
{
    Process g_KernelProcess(
        "atrikrnl",
        nullptr,
        0,
        &g_SystemActivity,
        TaskType::TaskTypeSystemProcess
    );
    Scheduler *g_Scheduler;

    Process* GetCurrentProcess() {
        Thread *currentThread = GetCPULocal()->currentThread;

        return Objects::IsNull(currentThread) ?
                nullptr :
                currentThread->m_Parent;
    }
    
    void Initialize()
    {
        g_Scheduler = new Scheduler();
        
        g_Scheduler->Register(&g_KernelProcess);
    }

    void Schedule()
    {
        
    }
}