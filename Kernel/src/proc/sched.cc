#include <proc/proc.h>
#include <mm/kmalloc.h>
#include <libkern/objects.h>

#if defined(ARCH_X86_64)

#include <Arch/x86_64/cpu.h>

#endif

namespace Task
{
    Process g_kernelProcess = Process();

    Process *processList[65536]; /* Max of 65536 */
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
    }

    void Schedule()
    {
        
    }
}