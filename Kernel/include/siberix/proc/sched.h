#pragma once

#include <common/typedefs.h>
#include <siberix/device/types.h>
#include <siberix/proc/process.h>
#include <utils/array.h>

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
    Scheduler(Process* kernelProcess);
    ~Scheduler();

    void         switchThread(Thread* newThread);
    u64          getTimeSlice();
    void         setTimeSlice(u64 ts);
    bool         addProcess(Process* process);
    bool         addProcess(Process* process, u8 priority);
    Process*     getProcessById(u16 pid);
    Process*     getKernelProcess();
    ThreadQueue& getThreadQueue();

    u32 nextPID() { return m_nextPID++; }

private:
    u32         m_nextPID = 1;
    u64         m_timeSlice;
    Process*    m_kernelProcess;
    Process*    m_processList[65536];
    ThreadQueue m_queue;
};

Process* thisProcess();
Thread*  thisThread();