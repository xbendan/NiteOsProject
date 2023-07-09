#include <common/logger.h>

#include <siberix/core/runtimes.hpp>
#include <siberix/proc/process.hpp>

Process::Process();
Process::Process(const char *_name,
                 File *_file,
                 u32 _processId,
                 Activity *_activity,
                 TaskType _type)
    : name(_name),
      processId(_processId),
{
    if (_name == nullptr) {
        Logger::getLogger("process").warn(
            "Trying to create process with null name.");
        this->name = _file->name;
    }

    flags |= (ProcessFlagIdle);
    ListNode<Thread> *mainThread = runtime()->getScheduler()
}

~Process();