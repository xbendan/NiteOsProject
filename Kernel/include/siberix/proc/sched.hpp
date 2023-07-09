#include <common/typedefs.h>

#include <siberix/proc/process.hpp>

class Scheduler {
public:
    bool switchTask();
    ListNode<Thread>* createThread();

private:
    u32 nextPID = 1;
    u64 m_timeSlice;
    Process* m_kernelProcess;
};