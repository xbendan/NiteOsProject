#pragma once

#include <siberix/mem/address-space.h>
#include <siberix/proc/thread.h>
#include <stdcxx/linked-list.h>
#include <stdcxx/refptr.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>
#include <xtra-concurrent/spinlock.h>

namespace Kern::Task {
    enum class ProcessType : uint8_t
    {
        Kernel = 0,
        System,
        Driver,
        Service,
        Background,
        Application,
    };

    class Process
    {
    public:
        Process() = default;
        Process(Std::String<Utf8>  name,
                uint32_t           processId,
                ProcessType        type,
                Mem::AddressSpace* addressSpace)
          : m_name(name)
          , m_processId(0)
          , m_addressSpace(nullptr)
          , m_mainThread(nullptr)
          , m_nextThreadId(0)
        {
        }
        ~Process() = default;

        Std::String<Utf8>  getName() { return m_name; }
        uint32_t           getProcessId() { return m_processId; }
        void               setProcessId(uint32_t pid) { m_processId = pid; }
        ProcessType        getType() { return m_type; }
        Mem::AddressSpace* getAddressSpace() { return m_addressSpace; }
        Thread*            getMainThread() { return m_mainThread; }
        Std::LinkedList<Thread*>& threads() { return m_childrenThreadList; }

        bool isThreadOwned(Thread* thread)
        {
            return thread->getParent() == this;
        }

        RefPtr<Thread> createThread();

    private:
        Std::String<Utf8>  m_name;
        uint32_t           m_processId;
        ProcessType        m_type;
        Mem::AddressSpace* m_addressSpace;

        Spinlock m_lock{};
        Spinlock m_handleLock{};

        Thread*                  m_mainThread;
        Std::LinkedList<Thread*> m_childrenThreadList;

        uint64_t m_entryPoint;
        uint64_t m_heap;
        uint32_t m_nextThreadId;
    };
}