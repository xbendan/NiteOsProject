#pragma once

#include <common/typedefs.h>
#include <siberix/mm/addrspace.h>
#include <utils/linked_list.h>

enum class TaskPriority : u8 {
    Lowest   = 0,
    Low      = 1,
    Normal   = 2,
    High     = 3,
    Highest  = 4,
    RealTime = 5
};

enum class TaskState {
    Running = 0,
    Idle    = 1,
    Blocked = 2,
    Deleted = 3
};

enum class TaskType {
    System      = 0,
    Driver      = 1,
    Application = 2,
    Service     = 3,
    Background  = 4
};

class Process;

class Thread {
public:
    Thread(Process* process, u32 threadId);
    ~Thread();

    u32      m_threadId;  /* Thread ID, not duplicated in same progress */
    Process* m_parent;    /* Parent process, indicates the owner of this thread */
    Spinlock m_lock;      /* Thread lock */
    Spinlock m_stateLock; /* Thread state lock */

    bool m_isIdleThread;

    TaskPriority m_priority;                   /* The priority when scheduling */
    TaskState    m_state = TaskState::Running; /* Thread state */

    u32 m_timeSlice = 0;
};