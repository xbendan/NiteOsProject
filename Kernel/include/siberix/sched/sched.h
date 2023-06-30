#include <siberix/common/typedefs.h>

namespace Process
{
    class Scheduler {
    private:
        u32 nextPID = 1;
        u64 timeSlice;
    }
} // namespace Process
