#pragma once

#include <siberix/proc/thread.h>
#include <siberix/svc/svc-host.h>
#include <stdcxx/types.h>
namespace Kern::Hal {
    class CPU
    {
    public:
        enum class Signal : uint32_t
        {
            INIT  = 0,
            START = 1,
            HALT  = 2,
            RESET = 3,
            SCHED = 4,
            EOI   = 5
        };

        CPU() = delete;
        constexpr CPU(uint32_t processorId)
          : m_self(this)
          , m_processorId(processorId)
          , m_currentThread(nullptr)
          , m_idleThread(nullptr)
        {
        }

        virtual void sendSignal(Signal signal);

        virtual void enableInterrupts();
        virtual void disableInterrupts();

    private:
        CPU*                m_self;
        uint32_t            m_processorId;
        Kern::Task::Thread* m_currentThread;
        Kern::Task::Thread* m_idleThread;
    };

    class SmpSvcHost : Svc::ISvcHost
    {
    public:
        SmpSvcHost()
          : ISvcHost("KERN.SMP", nullptr)
        {
        }

        virtual uint32_t getCPUAmount() { return m_cpuAmount; }
        virtual CPU*     getCPUById(uint32_t processorId)
        {
            return m_cpus[processorId];
        }
        virtual CPU* current() = 0;

    private:
        uint32_t         m_cpuAmount;
        Std::Array<CPU*> m_cpus;
    };

    class IPowerManagement
    {
    public:
        virtual ~IPowerManagement() = default;

        virtual void shutdown() = 0;
        virtual void sleep()    = 0;
        virtual void wake()     = 0;
    };
}