#pragma once

#include <fs/vfs.h>
#include <utils/list.h>
#include <utils/spinlock.h>
#include <macros>

#ifdef ARCH_X86_64
    #include <Arch/x86_64/cpu.h>
    #include <Arch/x86_64/mmu.h>
#elif ARCH_AARCH64

#elif ARCH_RISCV

#endif

typedef uint32_t tid_t;
typedef uint32_t pid_t;

using namespace Memory::ManagementUnit;

namespace Task
{
    struct Thread;
    class Activity;

    enum TaskPriority
    {
        PriorityIdle = -1,
        PriorityLowest = 0,
        PriorityLow = 1,
        PriorityNormal = 2,
        PriorityHigh = 3,
        PriorityHighest = 4,
        PriorityRealTime = 5
    };

    enum TaskState
    {
        TaskStateRunning = 0,
        TaskStateIdle = 1,
        TaskStateBlocked = 2,
        TaskStateDeleted = 3
    };

    enum TaskType
    {
        TaskTypeSystemProcess = 0,
        TaskTypeSystemDriver = 1,
        TaskTypeApplication = 2,
        TaskTypeService = 3,
        TaskTypeBackground = 4
    };

    class Process
    {
    public:
        Process(
            const char                 *name, 
            Fs::File                   *sourceFile, 
            pid_t                       pid, 
            Activity                   *activity, 
            TaskType                    taskType);

        void Start();
        void Suspend();
        void Stop();

        /**
         * @brief Send a message to terminate a process
         * 
         * This function will insert a message of terminating to the message 
         * queue of targeting process. It generally tells the process to stop
         * working and save all changes. Resources will be released manually
         * by the process, so it's safe to stop a processunless the process 
         * cannot handle this message. In that situation, sending a terminate 
         * message won't do anything, call @code {.c}
         * KillProcess(struct Process *process)
         * @endcode instead.
         * 
         * @param process 
         */
        void Terminate(int stopCode);

        const char *m_Name;         /* Name of the process */
        const char *m_Publisher;    /* Name of the publisher */
        //char *m_FileName;     /* Executable file name of this process */
        const char *m_Package;      /* Package Name */
        pid_t m_ProcessId;    /* Process Id, 0~255 are reserved for kernel process */
        TaskType m_Type;       /* Current process type */
        Fs::File *m_FilePtr;         /* Pointer to the source file, can be NULL */
        Activity *m_Activity;    /* Pointer to the Activity */
        struct
        {
            bool m_IsFocus: 1;
            bool m_IsIdle: 1;
            uint32_t __reserved__: 30;
        } __attribute__((packed));
        uint16_t m_Handles;   /* Register handles amount */
        //uint16_t m_Cores;     /* CPU Cores currently running on */
        struct
        { // Memory
            //uint64_t m_MemoryAllocated;     /* Memory allocated for this process */
            //uint32_t m_MemoryCached;        /* Memory allocated and cached */
            uint32_t m_Pages;               /* Amount of 4K pages */
            uint32_t m_PresentPages;        /* Pages that is present now */
            uint32_t m_SwappedPages;        /* Pages that has been swapped into disks */
        };

        struct
        {
            Spinlock m_Lock;
            Spinlock m_HandleLock;
        };

        uint32_t m_NextThreadId;
        Thread *m_MainThread;
        LinkedList<Thread> m_ChildrenThreadList;

        uintptr_t m_EntryPoint;
        uintptr_t m_Heap;

        /* Architecture Fields */
        #ifdef ARCH_X86_64 
        Memory::ManagementUnit::VirtualPages *m_Pagemap;
        #elif ARCH_AARCH64

        #elif ARCH_RISCV

        #endif
    };

    struct ThreadBlocker
    {

    };

    struct Thread
    {
        tid_t m_ThreadId;                  /* Thread ID, not duplicated in same progress */
        Process *m_Parent;    /* Parent process, indicates the owner of this thread */
        Spinlock m_Lock;            /* Thread lock */
        Spinlock m_StateLock;       /* Thread state lock */

        struct
        {
            uint32_t esp0;
            uint32_t ss0;
        };

        void *m_UserStack;
        void *m_UserStackBase;
        void *m_KernelStack;
        void *m_KernelStackBase;

        uint64_t m_FsBase;

        registers_t m_Registers;  
        registers_t m_LastSyscall;

        TaskPriority m_ThreadPriority;       /* The priority when scheduling */
        TaskState m_ThreadState = TaskState::TaskStateRunning;          /* Thread state */

        uint32_t m_TimeSlice = 0;

        Thread(Process *process)
          : m_Parent(process),
            m_ThreadId(process->m_NextThreadId++),
            m_ThreadPriority(TaskPriority::PriorityNormal),
            m_ThreadState(TaskState::TaskStateRunning) { }
    };
} // namespace Process
