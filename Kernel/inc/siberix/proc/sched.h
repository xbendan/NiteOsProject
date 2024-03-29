#pragma once

#include <siberix/fs/file.h>
#include <siberix/proc/process.h>
#include <stdcxx/array.h>
#include <stdcxx/refptr.h>
#include <stdcxx/types.h>

namespace Kern::Task {
    class Scheduler
    {
    public:
        Scheduler()
          : m_nextPID(0)
        {
        }
        ~Scheduler() = default;

        uint32_t getNextPID() { return m_nextPID++; }

    private:
        uint32_t m_nextPID;
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
        RefPtr<Process> createProcess(
          Std::String<Utf8> name,
          ProcessType       type = ProcessType::Background);
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
        template <typename... Args>
        RefPtr<Process> createProcessEx( //
          Std::String<Utf8> name,
          ProcessType       type,
          Io::File*         file,
          Io::Directory*    workingDirectory,
          Args&&... launchArgs);
        /**
         * @brief Create a thread with specified parent process
         *
         * @param process
         * @return Thread*
         */
        Thread*         createThread(Process* process);
        Thread*         createThreadEx(Process*          process,
                                       Std::String<Utf8> name,
                                       uint64_t          entryPoint,
                                       uint64_t          stackSize);
        Thread*         createIdleThread();
    };
}