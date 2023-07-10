#include <arch/x86_64/types.h>
#include <common/typedefs.h>
#include <utils/array.h>

#include <siberix/device/types.hpp>
#include <siberix/proc/process.hpp>

class Scheduler {
public:
    Scheduler();
    ~Scheduler();

    bool              switchTask();
    ListNode<Thread>* createThread();
    inline Cpu*       cpu(u8 id) { return m_cpus[id]; }

private:
    u32      nextPID = 1;
    u64      m_timeSlice;
    Process* m_kernelProcess;
    Process* m_processList[65536];
    Cpu*     m_cpus[256];
};