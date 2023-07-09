#include <siberix/fs/vfs.hpp>

enum ProcessFlags { ProcessFlagIdle = 0x01 };

class Process {
public:
    Process();
    Process(const char *name,
            Fs::File *file,
            u32 processId,
            Activity *activity,
            TaskType type);
    ~Process();

    void start();
    void suspend();
    void stop();

    bool isIdle() { return (flags & 0x01); }

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

protected:
    const char *name; /* Name of the process */
    // const char *publisher; /* Name of the publisher */
    // const char *package;   /* Package Name */
    u32 processId;      /* Process Id, 0~255 are reserved for kernel process */
    TaskType type;      /* Current process type */
    File *file;         /* Pointer to the source file, can be NULL */
    Activity *activity; /* Pointer to the Activity */

    u32 flags;
    u16 handles; /* Register handles amount */

    struct {
        Spinlock lock;
        Spinlock handleLock;
    };

    Thread *mainThread;
    LinkedList<Thread> childrenThreadList;

    u64 entryPoint;
    u64 heap;
    u32 nextThreadId;

    AddressSpace *addressSpace;

    friend Thread;
    friend Scheduler;
};