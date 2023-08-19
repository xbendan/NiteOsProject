#include <siberix/proc/process.h>
#include <siberix/proc/types.h>

Thread::Thread(Process* process)
    : m_threadId(process->m_nextThreadId++),
      m_parent(process),
      m_priority(TaskPriority::Normal),
      m_state(TaskState::Running) {}