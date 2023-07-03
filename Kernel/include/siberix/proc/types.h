#include <common/typedefs.h>
#include <siberix/mm/addrspace.hpp>
#include <utils/linked_list.h>

namespace Process
{
    enum TaskPriority {
        PriorityLowest = 0,
        PriorityLow = 1,
        PriorityNormal = 2,
        PriorityHigh = 3,
        PriorityHighest = 4,
        PriorityRealTime = 5
    };

    enum TaskState { TaskStateRunning = 0, TaskStateIdle = 1, TaskStateBlocked = 2, TaskStateDeleted = 3 };

    enum TaskType {
        TaskTypeSystemProcess = 0,
        TaskTypeSystemDriver = 1,
        TaskTypeApplication = 2,
        TaskTypeService = 3,
        TaskTypeBackground = 4
    };

    class Activity;

    class Process {
    public:
        Process();
        Process(const char* name, Fs::File* file, u32 processId, Activity* activity, TaskType type);
        ~Process();

        void start();
        void suspend();
        void stop();

        /**
         * @brief Send a message to terminate a process
         *
         * This function will insert a message of terminating to the message
         * queue of targeting process. It generally tells the process to stop
         * working and save all changes. Resources will be released manually
         * by the process, so it's safe to stop a process unless the process
         * cannot handle this message. In that situation, sending a terminate
         * message won't do anything, call @code {.c}
         * KillProcess(struct Process *process)
         * @endcode instead.
         *
         * @param process
         */
        void terminate(int stopCode);

        const char* name;      /* Name of the process */
        const char* publisher; /* Name of the publisher */
        const char* package;   /* Package Name */
        u32 processId;         /* Process Id, 0~255 are reserved for kernel process */
        TaskType type;         /* Current process type */
        Fs::File* file;        /* Pointer to the source file, can be NULL */
        Activity* activity;    /* Pointer to the Activity */

        bool isIdle() { return (flags & 0x01); }

        u32 flags;
        u16 handles; /* Register handles amount */

        struct {
            Spinlock lock;
            Spinlock handleLock;
        };

        Thread* mainThread;
        LinkedList<Thread> childrenThreadList;

        u64 entryPoint;
        u64 heap;

        AddressSpace* addressSpace;

    protected:
        u32 nextThreadId;

        friend Thread;
        friend Scheduler;
    };

    struct ThreadBlocker {};

    struct Thread {
        u32 threadId;       /* Thread ID, not duplicated in same progress */
        Process* parent;    /* Parent process, indicates the owner of this thread */
        Spinlock lock;      /* Thread lock */
        Spinlock stateLock; /* Thread state lock */

        struct {
            u32 esp0;
            u32 ss0;
        };

        void* userStack;
        void* userStack;
        void* kernelStack;
        void* kernelStackBase;

        uint64_t fsBase;
        void* fxState;

        RegisterContext registers;
        RegisterContext lastSyscall;

        TaskPriority priority;              /* The priority when scheduling */
        TaskState state = TaskStateRunning; /* Thread state */

        u32 timeSlice = 0;

        Thread(Process* process) :
            threadId(process->nextThreadId++), parent(process), priority(PriorityNormal), state(TaskStateRunning) {}
    };

    class Activity {
    public:
        Activity(const char* name, Process* proc);
        Activity(Process* proc);

        void add(Process* proc);

    private:
        const char* name;
        const char* desc;
        LinkedList<Process*> childrenProcessList;
    };
} // namespace Process