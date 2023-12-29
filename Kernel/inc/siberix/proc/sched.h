#include <siberix/fs/file.h>
#include <siberix/proc/process.h>
#include <stdcxx/array.h>
#include <stdcxx/refptr.h>
#include <stdcxx/types.h>

namespace Kern::Task {
    class Scheduler
    {
    private:
        uint32_t m_nextPID;
    };

    class ITaskHost
    {
    public:
        virtual ~ITaskHost() = default;

        Scheduler*      getScheduler() { return m_scheduler; }
        ProcessFactory* getProcessFactory() { return m_processFactory; }
        RefPtr<Process> getProcess(uint32_t pid) { return m_processes[pid]; }
        RefPtr<Process> getProcess(Std::String<Utf8> name);
        RefPtr<Process> getProcess(Std::UUID uuid);

    private:
        Scheduler*                  m_scheduler;
        ProcessFactory*             m_processFactory;
        Std::Array<RefPtr<Process>> m_processes;
    };

    class ProcessFactory
    {
    public:
        /**
         * @brief Create a Process with specified name
         * The process will not be started until it has a valid entry point
         * with a existed file.
         *
         * @param name
         * @return Process* pointer to the created process
         */
        RefPtr<Process> createProcess(Std::String<Utf8> name);
        /**
         * @brief Create an idle process
         * The idle process will be started immediately after it is created,
         * but consume no resources.
         *
         * @return Process*
         */
        RefPtr<Process> createIdleProcess();
        /**
         * @brief Create a process with full arguments
         *
         * @param name
         * @return Process*
         */
        RefPtr<Process> createProcessEx(
          Std::String<Utf8>              name,
          Io::File*                      file,
          Io::Directory*                 workingDirectory,
          Std::Array<Std::String<Utf8>>* launchArgs);
        /**
         * @brief Create a thread with specified parent process
         *
         * @param process
         * @return Thread*
         */
        Thread* createThread(Process* process);
        Thread* createIdleThread();

    private:
        ProcessFactory(ITaskHost* host)
          : m_host(host)
        {
        }

        friend ITaskHost;
        ITaskHost* m_host;
    };
}