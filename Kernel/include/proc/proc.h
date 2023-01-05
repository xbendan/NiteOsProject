#pragma once

#include <fs/vfs.h>
#include <proc/activity.h>
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
typedef uint16_t pid_t;

using namespace Utils;

namespace Proc
{
    typedef enum TaskPriority
    {
        PriorityIdle = -1,
        PriorityLowest = 0,
        PriorityLow = 1,
        PriorityNormal = 2,
        PriorityHigh = 3,
        PriorityHighest = 4,
        PriorityRealTime = 5
    } task_priority_t;

    typedef enum TaskState
    {
        TaskStateRunning = 0,
        TaskStateIdle = 1,
    } task_state_t;

    typedef enum TaskType
    {
        TaskTypeSystemProcess = 0,
        TaskTypeSystemDriver = 1,
        TaskTypeApplication = 2,
        TaskTypeService = 3,
        TaskTypeBackground = 4
    } task_type_t;

    typedef enum ThreadState
    {
        ThreadStateAbolished = 0,
        ThreadStateRunning = 1,
        ThreadStateBlocked = 2
    } thread_state_t;

    typedef struct Thread
    {
        tid_t m_ThreadId;                  /* Thread ID, not duplicated in same progress */
        proc_t *m_Parent;    /* Parent process, indicates the owner of this thread */
        spinlock_t m_Lock;            /* Thread lock */
        spinlock_t m_StateLock;       /* Thread state lock */

        struct
        {
            uint32_t esp0;
            uint32_t ss0;
        };

        void *m_UserStack;
        void *m_UserStackLimit;
        void *m_KernelStack;
        void *m_KernelStackLimit;

        registers_t m_Registers;  
        registers_t m_LastSyscall;

        uint8_t m_ThreadPriority;       /* The priority when scheduling */
        uint8_t m_ThreadState;          /* Thread state */
    } thread_t;

    typedef class Process
    {
    public:
        Process(char *name, Fs::File *sourceFile, pid_t pid, Proc::Activity *activity, TaskType taskType)
          : m_Name(name),
            m_FilePtr(sourceFile),
            m_ProcessId(pid),
            m_Activity(activity),
            m_Type(taskType)
        {

        }

        Process()
          : m_Name("atrikrnl"),
            m_Package("org.myosotisdev.niteos"),
            m_ProcessId(0),
            m_Type(TaskTypeSystemProcess)
        {

        }

        void Start()
        {

        }

        void Pause()
        {
            
        }

        void Stop()
        {

        }

        void Terminate(int stopCode)
        {

        }

        char *m_Name;         /* Name of the process */
        char *m_Publisher;    /* Name of the publisher */
        //char *m_FileName;     /* Executable file name of this process */
        char *m_Package;      /* Package Name */
        pid_t m_ProcessId;    /* Process Id, 0~255 are reserved for kernel process */
        task_type_t m_Type;       /* Current process type */
        Fs::file_t *m_FilePtr;         /* Pointer to the source file, can be NULL */
        activity_t *m_Activity;    /* Pointer to the Activity */
        union
        { // Flags
            uint32_t __flag_size__;
            struct
            {
                bool m_IsFocus: 1;
                bool m_IsIdle: 1;
                uint32_t __reserved__: 30;
            } __attribute__((packed));
        };
        uint16_t m_Threads;   /* Amount of threads kept by this process*/
        uint16_t m_Handles;   /* Register handles amount */
        //uint16_t cores;     /* CPU Cores currently running on */
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
            spinlock_t m_Lock;
            spinlock_t m_HandleLock;
        };

        uint32_t m_NextThreadId;
        thread_t *m_MainThread;
        LinkedList<Thread> m_ChildrenThreadList;

        uintptr_t m_EntryPoint;
        uintptr_t m_Heap;

        /* Architecture Fields */
        #ifdef ARCH_X86_64
        Memory::ManagementUnit::VirtualPages *m_Pagemap;
        #elif ARCH_AARCH64

        #elif ARCH_RISCV

        #endif
    } proc_t;

    /**
     * @brief Create a Process object
     * 
     * @return struct Process* Pointer to new process object
     */
    proc_t *CreateProcess();

    /**
     * @brief Create a process from existing file
     * 
     * @param file Source file, must be executable
     * @return struct Process* Pointer to new process object
     */
    proc_t *CreateELFProcess(Fs::file_t *file);

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
    proc_t *CreateProcessEx(activity_t *activity, task_type_t type, Fs::file_t *file, char *name);

    /**
     * @brief Create a new thread object of specific process
     * 
     * @param process Parent process
     * @return struct Thread* Pointer to new thread
     */
    ListNode<thread_t> *CreateThread(proc_t *process);

    /**
     * @brief Forcely kill a process and clean all resources
     * 
     * Killing process is an irreversible operation that it stops the
     * process immediately and release all the resources, including allocated
     * memory, opened files, and other resources locked by this process.
     * All of these operation will be done by the kernel, it would not ask
     * process to do anything. On the other words, process will be unable to
     * save any changes.
     * 
     * @param process Process that will be killed
     */
    void KillProcess(proc_t *process);

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
    void TerminateProcess(proc_t *process);
} // namespace Process
