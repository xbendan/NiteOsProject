#include <mixins/base/string.h>
#include <sbrx/memory/address_space.h>
#include <sbrx/sched/thread.h>

using namespace mixins;

namespace sbrx::sched {
    class Process
    {
    public:
    private:
        uint32_t           m_processId;
        String<Utf8>       m_name;
        mem::AddressSpace* m_addressSpace;

        uint64_t m_entryPoint;
        uint64_t m_memHeap;
        uint64_t m_memStack;
        uint64_t m_nextThreadId;
    };
}