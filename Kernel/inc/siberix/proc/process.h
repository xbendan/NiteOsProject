#include <siberix/mem/address-space.h>
#include <siberix/proc/thread.h>
#include <stdcxx/linked-list.h>
#include <stdcxx/string.h>
#include <stdcxx/types.h>
#include <xtra-concurrent/spinlock.h>

namespace Kern::Task {
    class Process
    {
    public:
        Process();
        ~Process() = default;

    private:
        Std::String<Utf8>  m_name;
        uint32_t             m_processId;
        Mem::AddressSpace* m_addressSpace;

        struct
        {
            Xtra::Concurrent::Spinlock m_lock;
            Xtra::Concurrent::Spinlock m_handleLock;
        };

        Thread*                  m_mainThread;
        Std::LinkedList<Thread*> m_childrenThreadList;

        uint64_t m_entryPoint;
        uint64_t m_heap;
        uint32_t m_nextThreadId;
    };
}