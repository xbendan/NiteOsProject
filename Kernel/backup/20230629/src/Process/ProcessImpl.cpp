#include <Proc/Process.h>
#include <Proc/Scheduler.h>
#include <Proc/Activity.h>
#include <System.h>

namespace Task
{    
    Process::Process() {}

    Process::Process(
        const char                     *name,
        Fs::File                       *file,
        uint32_t                        processId,
        Activity                       *activity,
        TaskType                        type
    ) : m_Name(name),
        m_FileSource(file),
        m_ProcessId(processId),
        m_Activity(activity),
        m_Type(type)
    {
        if (Objects::IsNull((void *)name) && !Objects::IsNull(file)) {
            m_Name = file->m_Name;
        }

        m_Flags |= 0x01;

        if (m_Activity == nullptr)
        {
            this->m_Activity = new Activity(this);
        }

        ListNode<Thread> *mainThread = g_Scheduler->CreateThread(this);

        m_MainThread = &mainThread->obj;
        m_ChildrenThreadList.Add(mainThread);
    }

    // Process::Process(
    //     const char                     *name, 
    //     Fs::File                       *sourceFile, 
    //     pid_t                           pid, 
    //     Activity                       *activity, 
    //     TaskType                        taskType
    // ) : m_Name(name),
    //     m_FilePtr(sourceFile),
    //     m_ProcessId(pid),
    //     m_Activity(activity),
    //     m_Type(taskType)
    // {
    //     Activity::CreateIfNull(this);
    //     ListNode<Thread> *mainThread = g_Scheduler->CreateThread(this);

    //     m_MainThread = &mainThread->obj;
    //     m_ChildrenThreadList.Add(mainThread);
    // }

    Process::~Process()
    {
        
    }

    void Process::Start()
    {
        m_Flags &= ~0x01;
    }

    void Process::Suspend()
    {
        m_Flags |= 0x01;
    }

    void Process::Stop()
    {

    }
}