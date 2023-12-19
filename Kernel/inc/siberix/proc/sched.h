#include <siberix/proc/process.h>
#include <stdcxx/refptr.h>
#include <stdcxx/types.h>

namespace Kern::Task {
    class Scheduler
    {
    private:
        UInt32 m_nextPID;
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
        RefPtr<Process> createProcessEx(Std::String<Utf8> name);
        /**
         * @brief Create a thread with specified parent process
         *
         * @param process
         * @return Thread*
         */
        Thread*         createThread(Process* process);
        Thread*         createIdleThread();
    };
}