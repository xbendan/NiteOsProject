#include <siberix/proc/sched.h>

namespace Kern::Task {
    RefPtr<Process> ProcessFactory::createProcess(Std::String<Utf8> name,
                                                  ProcessType       type)
    {
        // Process* process = new Process(name, type);
    }

    RefPtr<Process> ProcessFactory::createIdleProcess() {}

    template <typename... Args>
    RefPtr<Process> ProcessFactory::createProcessEx(
      Std::String<Utf8> name,
      ProcessType       type,
      Io::File*         file,
      Io::Directory*    workingDirectory,
      Args&&... launchArgs)
    {
    }

    Thread* ProcessFactory::createThread(Process* process) {}

    Thread* ProcessFactory::createThreadEx(Process*          process,
                                           Std::String<Utf8> name,
                                           uint64_t          entryPoint,
                                           uint64_t          stackSize)
    {
    }

    Thread* ProcessFactory::createIdleThread() {}

}