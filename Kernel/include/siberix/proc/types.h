#include <common/typedefs.h>
#include <utils/linked_list.h>

#include <siberix/mm/addrspace.hpp>

enum TaskPriority {
    PriorityLowest   = 0,
    PriorityLow      = 1,
    PriorityNormal   = 2,
    PriorityHigh     = 3,
    PriorityHighest  = 4,
    PriorityRealTime = 5
};

enum TaskState {
    TaskStateRunning = 0,
    TaskStateIdle    = 1,
    TaskStateBlocked = 2,
    TaskStateDeleted = 3
};

enum TaskType {
    TaskTypeSystemProcess = 0,
    TaskTypeSystemDriver  = 1,
    TaskTypeApplication   = 2,
    TaskTypeService       = 3,
    TaskTypeBackground    = 4
};

struct Thread {
    u32      m_threadId; /* Thread ID, not duplicated in same progress */
    Process* m_parent; /* Parent process, indicates the owner of this thread */
    Spinlock m_lock;   /* Thread lock */
    Spinlock m_stateLock; /* Thread state lock */

    bool m_isIdleThread;

    struct {
        u32 esp0;
        u32 ss0;
    };

    u64   fsBase;
    void* fxState;

    void* m_userStack;
    void* m_userStackBase;
    void* m_kernelStack;
    void* m_kernelStackBase;

    RegisterContext m_registers;
    RegisterContext m_lastSyscall;

    TaskPriority m_priority;                 /* The priority when scheduling */
    TaskState    m_state = TaskStateRunning; /* Thread state */

    u32 m_timeSlice = 0;

    Thread(Process* process)
        : m_threadId(process->nextThreadId++),
          m_parent(process),
          m_priority(PriorityNormal),
          m_state(TaskStateRunning) {}
};