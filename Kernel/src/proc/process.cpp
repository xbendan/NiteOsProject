#include <common/logger.h>
#include <siberix/core/runtimes.h>

#include <siberix/proc/process.h>

Process::Process() {}

Process::Process(const char *_name, File *_file, u32 _processId, TaskType _type)
    : m_name(_name),
      m_processId(_processId),
{
    if (_name == nullptr) {
        Logger::getLogger("process").warn(
            "Trying to create process with null name.");
        this->m_name = _file->getName();
    }

    m_flags      |= (ProcessFlagIdle);
    m_mainThread  = new Thread(this);
    m_childrenThreadList.add(new ListNode<Thread *>(m_mainThread));
}

Process::~Process() {}