#include <proc/proc.h>
#include <proc/sched.h>
#include <proc/activity.h>

namespace Task
{
    Process::Process(
        const char                     *name, 
        Fs::File                       *sourceFile, 
        pid_t                           pid, 
        Activity                       *activity, 
        TaskType                        taskType
    ) : m_Name(name),
        m_FilePtr(sourceFile),
        m_ProcessId(pid),
        m_Activity(activity),
        m_Type(taskType),
        m_IsIdle(true)
    {
        Activity::CreateIfNull(*this);
        ListNode<Thread> *mainThread = g_Scheduler->CreateThread(this);

        m_MainThread = &mainThread->obj;
        m_ChildrenThreadList.Add(mainThread);
    }

    void Process::Start()
    {
        this->m_IsIdle = false;
    }

    void Process::Suspend()
    {
        this->m_IsIdle = true;
    }

    void Process::Stop()
    {

    }
}