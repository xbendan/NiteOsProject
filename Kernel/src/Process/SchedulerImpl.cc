#include <Proc/Scheduler.h>
#include <Proc/Activity.h>
#include <libkern/objects.h>
#include <Mem/KMemAlloc.h>

#ifdef ARCH_X86_64
    #include <Arch/x86_64/CPU.h>
#endif

namespace Task
{
    Scheduler::Scheduler()
    {
        Register(&g_KernelProcess);
    }

    Scheduler::~Scheduler()
    {

    }

    uint32_t Scheduler::NextPID()
    {
        uint32_t _next;

        do {
            _next = m_NextPID++;
        } while (GetProcessById(_next) != nullptr);

        return _next;
    }

    bool Scheduler::Register(Process *process)
    {
        Process **processInList = &(this->m_ProcessList[process->m_ProcessId]);

        if (!Objects::IsNull(*processInList)
            && *processInList != process)
        {
            return false;
        }

        *processInList = process;

        System::Out("Process registered. PID=%u", process->m_ProcessId);

        process->Start();
        return true;
    }

    void Scheduler::Schedule()
    {
        CPU *cpu = GetCPULocal();

        //if ()
    }

    Process *Scheduler::CreateProcess(
        const char                     *name,
        Activity                       *activity
    ) {
        return this->CreateProcessEx(name, activity, TaskType::TaskTypeBackground, nullptr);
    }

    Process *Scheduler::CreateIdleProcess()
    {
        return this->CreateProcessEx("/", nullptr, TaskType::TaskTypeSystemProcess, nullptr);
    }

    Process *Scheduler::CreateELFProcess(
        Fs::File                       *file
    ) {
        return CreateProcessEx(file->m_Name, nullptr, TaskTypeBackground, file);
    }

    Process *Scheduler::CreateProcessEx(
        const char                     *name,
        Activity                       *activity, 
        TaskType                        type, 
        Fs::File                       *file
    ) {
        Process *proc = new Process(name, file, this->NextPID(), activity, type);
        Register(proc);
        return proc;
    }

    ListNode<Thread> *Scheduler::CreateThread(
        Process                        *proc
    ) {
        if (Objects::IsNull(&proc))
            return nullptr;

        ListNode<Thread> *thread = new ListNode<Thread>(Thread(proc));

        return thread;
    }
} // namespace Process
