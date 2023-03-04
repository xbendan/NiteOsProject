#include <proc/proc.h>
#include <proc/sched.h>
#include <libkern/objects.h>

namespace Task
{
    Process *CreateProcess()
    {
    }

    Process *CreateIdleProcess()
    {
        Process *proc = new Process("/", nullptr, g_Scheduler->NextPID(), )
    }

    Process *CreateELFProcess(Fs::File *file)
    {
        return CreateProcessEx(nullptr, TaskTypeBackground, file, file->name);
    }

    Process *CreateProcessEx(
        Activity               *activity, 
        TaskType                type, 
        Fs::File               *file, 
        char                   *name
    ) {
        Process *proc = (Process *)kmalloc(sizeof(Process));

        *proc = Process(name, file, GetNextPID(), activity, type);

        strcpy(proc->m_Name, name);

        if (activity == NULL)
            proc->m_Activity = CreateActivity(proc->m_Name);

        // InitLock(&newProc->m_Lock);
        // InitLock(&newProc->m_HandleLock);

        Utils::ListNode<Thread> *thread = CreateThread(proc);
        proc->m_MainThread = &thread->obj;
        proc->m_ChildrenThreadList.Add(thread);

        return proc;
    }

    Utils::ListNode<Thread> *CreateThread(
        Process                        *proc
    ) {
        if (Objects::IsNull(proc))
            return nullptr;

        Utils::ListNode<Thread> *thread = reinterpret_cast<Utils::ListNode<Thread> *>(kmalloc(sizeof(Utils::ListNode<Thread>)));

        thread->obj = (Thread){
            .m_ThreadId = proc->m_NextThreadId++,
            //.m_Parent = proc,
            .m_ThreadPriority = PriorityNormal,
            .m_ThreadState = TaskStateIdle
        };

        return thread;
    }

    Activity *CreateActivity(
        const char                     *name
    ) {
    }
} // namespace Process
