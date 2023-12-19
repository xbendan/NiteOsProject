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
        UInt32             m_processId;
        Mem::AddressSpace* m_addressSpace;

        struct
        {
            Xtra::Concurrent::Spinlock m_lock;
            Xtra::Concurrent::Spinlock m_handleLock;
        };

        Thread*                  m_mainThread;
        Std::LinkedList<Thread*> m_childrenThreadList;

        UInt64 m_entryPoint;
        UInt64 m_heap;
        UInt32 m_nextThreadId;
    };
}