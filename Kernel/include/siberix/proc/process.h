#pragma once

#include <siberix/fs/vfs.h>
#include <siberix/proc/types.h>

enum ProcessFlags {
    ProcessFlagIdle = 0x01
};

class Process {
public:
    Process();
    Process(const char *name, File *file, u32 processId, TaskType type);
    ~Process();

    void start();
    void suspend();
    void stop();

    bool isIdle() { return (m_flags & 0x01); }

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

    const char   *getName() { return m_name; }
    u32           getProcessId() { return m_processId; }
    TaskType      getType() { return m_type; }
    File         *getFile() { return m_file; }
    AddressSpace *getAddressSpace() { return m_addressSpace; }

protected:
    const char *m_name;      /* Name of the process */
    u32         m_processId; /* Process Id, 0~255 are reserved for kernel process */
    TaskType    m_type;      /* Current process type */
    File       *m_file;      /* Pointer to the source file, can be NULL */
    // Activity *m_activity; /* Pointer to the Activity */

    u32 m_flags;
    u16 m_handles; /* Register handles amount */

    struct {
        Spinlock m_lock;
        Spinlock m_handleLock;
    };

    Thread              *m_mainThread;
    LinkedList<Thread *> m_childrenThreadList;

    u64 m_entryPoint;
    u64 m_heap;
    u32 m_nextThreadId;

    AddressSpace *m_addressSpace;

    friend Thread;
};