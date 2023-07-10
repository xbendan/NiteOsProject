#include <common/typedefs.h>
#include <utils/linked_list.h>

#include <siberix/mm/addrspace.hpp>

enum TaskPriority {
    PriorityLowest = 0,
    PriorityLow = 1,
    PriorityNormal = 2,
    PriorityHigh = 3,
    PriorityHighest = 4,
    PriorityRealTime = 5
};

enum TaskState {
    TaskStateRunning = 0,
    TaskStateIdle = 1,
    TaskStateBlocked = 2,
    TaskStateDeleted = 3
};

enum TaskType {
    TaskTypeSystemProcess = 0,
    TaskTypeSystemDriver = 1,
    TaskTypeApplication = 2,
    TaskTypeService = 3,
    TaskTypeBackground = 4
};

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

    Thread(Process* process)
        : threadId(process->nextThreadId++),
          parent(process),
          priority(PriorityNormal),
          state(TaskStateRunning) {}
};

namespace Process {

    class Activity;

    struct ThreadBlocker {};

}  // namespace Process