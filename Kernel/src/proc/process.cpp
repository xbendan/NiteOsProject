#include <common/logger.h>

#include <siberix/core/runtimes.h>
#include <siberix/proc/process.hpp>

Process::Process() {}

Process::Process(const char *_name, File *_file, u32 _processId, TaskType _type)
    : name(_name),
      processId(_processId),
{
    if (_name == nullptr) {
        Logger::getLogger("process").warn(
            "Trying to create process with null name.");
        this->name = _file->name;
    }

    flags |= (ProcessFlagIdle);
    m_mainThread = new Thread(this);
    m_childrenThreadList.add(new ListNode<Thread *>(m_mainThread));
}

~Process() {}