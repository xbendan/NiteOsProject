#include <arch/x86_64/types.h>
#include <siberix/proc/process.h>

X64Thread::X64Thread()
    : Thread(nullptr, 0) {}

X64Thread::X64Thread(Process *process, u32 threadId)
    : Thread(process, threadId) {}

X64Thread::~X64Thread() {}

ProcessFactory::ProcessFactory() {}

ProcessFactory::~ProcessFactory() {}

Process *ProcessFactory::createProcess(const char *name) {
    Process *process = new Process(name, nullptr, 0, TaskType::Background);
    siberix()->getScheduler()->addProcess(process);
    return process;
}

Process *ProcessFactory::createElfProcess(File *file) {
    Process *process = new Process(file->getName(), file, 0, TaskType::Background);
    siberix()->getScheduler()->addProcess(process);
    return process;
}

Process *ProcessFactory::createIdleProcess() {
    Process *process = new Process("idle", nullptr, 0, TaskType::System);
    siberix()->getScheduler()->addProcess(process);
    process->m_flags |= ProcessFlagIdle;
    return process;
}

Process *ProcessFactory::createProcessEx(const char *name, File *file, TaskType type) {
    Process *process = new Process(name, file, siberix()->getScheduler()->nextPID(), type);
    siberix()->getScheduler()->addProcess(process);
    return process;
}

Thread *ProcessFactory::createThread(Process *process) {
    return new X64Thread(process, process->m_nextThreadId++);
}

Thread *ProcessFactory::createIdleThread() { return new X64Thread(); }