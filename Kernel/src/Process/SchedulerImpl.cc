#include <proc/proc.h>
#include <proc/sched.h>
#include <proc/activity.h>
#include <libkern/objects.h>

#ifdef ARCH_X86_64
    #include <Arch/x86_64/cpu.h>
#endif

namespace Task
{
    void Scheduler::Schedule()
    {
        ProcessorCore *cpu = GetCPULocal();

        if ()
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
