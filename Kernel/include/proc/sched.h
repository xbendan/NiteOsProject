#include <proc/proc.h>
#include <libkern/ref.h>

#define MAX_PROCESS_AMOUNT 65536

namespace Task
{
    class Scheduler {
    private:
        /* PID 0 = Kernel, it will never be assigned */
        pid_t m_NextPID = 1;
        Process *m_IdleProcess;
        Process *m_ProcessList[MAX_PROCESS_AMOUNT];
        uint64_t m_TimeSlice;

    public:
        uint16_t m_Processes = 0;
        uint64_t m_Threads = 0;

        Scheduler()
          : m_IdleProcess(CreateIdleProcess()) {
            
        }

        ~Scheduler() {

        }

        pid_t NextPID() {
            pid_t _next;

            do {
                _next = m_NextPID++;
            } while (Objects::IsNull(GetProcessById(_next)));

            return _next;
        }

        void Schedule() {

        }

        Process *GetProcessById(pid_t id) {
            return m_ProcessList[id];
        }

        /**
         * @brief Create a Process object
         * 
         * @return struct Process* Pointer to new process object
         */
        Process *CreateProcess();

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
            Fs::file_t                     *file);

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
            activity_t                     *activity, 
            task_type_t                     type, 
            Fs::file_t                     *file, 
            char                           *name);

        /**
         * @brief Create a new thread object of specific process
         * 
         * @param process Parent process
         * @return struct Thread* Pointer to new thread
         */
        ListNode<Thread> *CreateThread(
            Process                        *process);
    };

    extern Scheduler *g_Scheduler;

    Process *GetCurrentProcess();
    
    void Initialize();
    void Schedule();
}