#include <proc/proc.h>
#include <mm/kmalloc.h>
#include <libkern/objects.h>

namespace Proc
{
    Process g_kernelProcess = Process();
    Process *g_currentProcess[MAX_CPU_AMOUNT];

    pid_t currentProcessId = 0;
    Process *processListByPid[65536];

    Process* GetKernelProcess() { return &g_kernelProcess; }
    Process* GetCurrentProcess() { return g_currentProcess[CPU_CORE_ID]; }

    pid_t GetNextPID()
    {
        // TODO ...
        return 0;
    }
    
    Process *GetProcessById(pid_t pid)
    {
        if(pid > 65535)
            return NULL;
        return processListByPid[pid];
    }
    
    void Initialize()
    {

    }

    void Schedule()
    {
        
    }

    Process *CreateProcess()
    {

    }

    Process *CreateELFProcess(Fs::File *file) {
        return CreateProcessEx(NULL, TaskTypeBackground, file, file->name);
    }

    Process *CreateProcessEx(Activity *activity, TaskType type, Fs::File *file, char *name) {
        Process *newProc = (Process *) kmalloc(sizeof(Process));

        *newProc = Process(name, file, GetNextPID(), activity, type);

        /* {
            .m_Name = (char *) kmalloc(strlen(name)),
            .m_Publisher = NULL,
            .m_Package = NULL,
            .m_ProcessId = GetNextPID(),
            .m_Type = type,
            .m_FilePtr = file,
            .m_Activity = activity,
            .m_IsFocus = false,
            .m_IsIdle = true,
            .m_Threads = 1,
            .m_Handles = 0,
            .m_MemoryAllocated = 0,
            .m_MemoryCached = 0,
            .m_Pages = 0,
            .m_PresentPages = 0,
            .m_SwappedPages = 0,
            .m_NextThreadId = 0,
            .m_MainThread = NULL,
            .m_EntryPoint = 0x0, // Need a default entry point here
            .m_Heap = 0x0

            #ifdef ARCH_X86_64
            .m_Pagemap = (void *) Memory::CreatePagemap();
            #elif ARCH_AARCH64

            #elif ARCH_RISCV

            #endif
        }; */
        
        strcpy(newProc->m_Name, name);
        
        if(activity == NULL)
            newProc->m_Activity = NewActivity(newProc->m_Name);

        //InitLock(&newProc->m_Lock);
        //InitLock(&newProc->m_HandleLock);

        Utils::ListNode<Thread> *thread = CreateThread(newProc);
        newProc->m_MainThread = &thread->obj;
        newProc->m_ChildrenThreadList.Add(thread);
    }

    Utils::ListNode<Thread> *CreateThread(Process *proc)
    {
        if (Objects::IsNull(proc))
            return nullptr;

        Utils::ListNode<Thread> *newThread = reinterpret_cast<Utils::ListNode<Thread> *>(kmalloc(sizeof(Utils::ListNode<Thread>)));

        newThread->obj = (Thread)
        {
            .m_ThreadId = proc->m_NextThreadId++,
            //.m_Parent = proc,
            .m_ThreadPriority = PriorityNormal,
            .m_ThreadState = TaskStateIdle
        };
        
        return newThread;
    }

    Activity *NewActivity(const char *name)
    {
        
    }
}