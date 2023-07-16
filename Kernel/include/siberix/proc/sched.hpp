#include <arch/x86_64/types.h>
#include <common/typedefs.h>
#include <utils/array.h>

#include <siberix/device/types.hpp>
#include <siberix/proc/process.hpp>

class ThreadQueue {
public:
    void    computeNewPrioirty();
    void    computeNewPriority(u8 dynamic);
    void    balanceQueues();
    Thread& next();
    Thread& next(u8 priority);

private:
    LinkedList<Thread&> m_priorityList;
};

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    bool              switchTask(Thread* newThread);
    ListNode<Thread>* createThread();
    inline Cpu*       cpu(u8 id) { return m_cpus[id]; }
    u32               getNextPID();
    u64               getTimeSlice();
    void              setTimeSlice(u64 ts);
    Process*          getProcessById(u16 pid);
    ThreadQueue&      getThreadQueue();

private:
    u32         nextPID = 1;
    u64         m_timeSlice;
    Process*    m_processList[65536];
    Cpu*        m_cpus[256];
    ThreadQueue m_queue;
};