#include <siberix/hwtypes.h>
#include <siberix/mem/address-space.h>
#include <siberix/mem/mem-alloc.h>
#include <siberix/mem/mem.h>
#include <siberix/proc/sched.h>
#include <stdcxx/types.h>

namespace Kern {
    namespace Main {

    }

    /**
     * @brief Kernel-mode service
     *
     */
    class MainHost
    {
    public:
        bool isInitialized() { return m_isInitialized; }

        /* --- Memory --- */
        uint64_t alloc4KPages(uint64_t amount);
        uint64_t alloc4KPages(uint64_t           amount,
                              Mem::AddressSpace* addressSpace,
                              Mem::Page4K**      pageReference);
        void     free4KPages(uint64_t address, uint64_t amount);

        inline Mem::IMemHost* getMemHost() { return &m_memHost; }

    private:
        bool                   m_isInitialized;
        Mem::IMemHost          m_memHost;
        Task::IProcessHost*    m_processHost;
        Hal::ICpuHost*         m_cpuHost;
        Hal::IPowerManagement* m_powerManagement;
    };
}

Kern::MainHost*
kern();