#include <siberix/fs/vfs.h>
#include <siberix/proc/types.h>

#include <siberix/proc/process.h>

namespace proc {
    class ProcessFactory {
        virtual Process* createProcess(const char* name);
        virtual Process* createElfProcess(File* file);
        virtual Process* createIdleProcess();
        virtual Process* createProcessEx(const char* name);
        virtual Thread*  createThread(Process* process);
    };
}  // namespace proc