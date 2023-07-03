#include <siberix/fs/vfs.hpp>
#include <siberix/proc/types.h>

namespace Process
{
    class ProcessFactory {
        virtual Process* createProcess(const char* name, Activity activity);
        virtual Process* createElfProcess(Fs::File* file);
        virtual Process* createIdleProcess();
        virtual Process* createProcessEx(const char* name, )
    };
} // namespace Process