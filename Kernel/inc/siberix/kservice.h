#include <siberix/device/device-controller.h>
#include <siberix/mem/address-space.h>
#include <siberix/mem/mem-alloc.h>
#include <siberix/mem/page.h>
#include <stdcxx/types.h>

namespace Kern {
    /**
     * @brief Kernel-mode service
     *
     */
    class KernelServiceMain
    {
    public:
        bool isInitialized() { return m_isInitialized; }

        /* --- Memory --- */
        uint64_t alloc4KPages(uint64_t amount);
        uint64_t alloc4KPages(uint64_t             amount,
                            Mem::AddressSpace* addressSpace,
                            Mem::Page4K**      pageReference);
        void   free4KPages(uint64_t address, uint64_t amount);

        uint64_t alloc(uint64_t size);
        void   free(uint64_t address);

    private:
        bool                      m_isInitialized;
        Mem::AddressSpace*        m_addressSpace;
        Mem::MemoryAllocatorBase*       m_memoryAlloc;
        Mem::MemoryAllocatorBase*       m_pagedAlloc;
        Device::DeviceManagement* m_deviceManagement;
    };
}

Kern::KernelServiceMain*
kern();