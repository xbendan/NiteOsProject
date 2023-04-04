#include <Proc/Process.h>
#include <libkern/ref.h>
#include <Utils/ArrayList.h>

#define MAX_PROCESS_AMOUNT 65536

namespace Task
{
    class Scheduler {
    private:
        /* PID 0 = Kernel, it will never be assigned */
        pid_t m_NextPID = 1;
        SizedArrayList<Process *, MAX_PROCESS_AMOUNT> m_ProcessList;
        uint64_t m_TimeSlice;

    public:
        Process *m_IdleProcess;

        Scheduler();
        ~Scheduler();

        pid_t NextPID();

        Process *GetProcessById(pid_t id) {
            return m_ProcessList[id];
        }

        bool Register(Process *process);

        void Schedule();

        void SwitchThread(Thread *newThread);

        /**
         * @brief Create a Process object
         * 
         * @return Process* Pointer to new process object
         */
        Process *CreateProcess(
            const char                 *name,
                  Activity             *activity);

        /**
         * @brief Create a Idle Process object
         * 
         * @return Process* 
         */
        Process *CreateIdleProcess();

        /**
         * @brief Create a process from existing file
         * 
         * @param file Source file, must be executable
         * @return struct Process* Pointer to new process object
         */
        Process *CreateELFProcess(
            Fs::File                 *file);

        /**
         * @brief Create a Process object with full arguments
         * 
         * A process will not be scheduled once it is created because the default
         * state of a process is TaskStateIdle.
         * Set it to TaskStateRunning to active it.
         * 
         * @param activity Activity the process belongs to
         * @param name Process name, use filename instead if its NULL, or <activity_name>-<pid>
         * @param file The file that the process created from
         * @return struct Process* Pointer to new process object
         */
        Process *CreateProcessEx(
            const char                       *name,
                  Activity                   *activity, 
                  TaskType                    type, 
                  Fs::File                   *file);

        /**
         * @brief Create a new thread object of specific process
         * 
         * @param process Parent process
         * @return struct Thread* Pointer to new thread
         */
        ListNode<Thread> *CreateThread(
            Process                        *process);
    };

    extern Process g_KernelProcess;
    extern Scheduler *g_Scheduler;

    Process *GetCurrentProcess();
}