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
        UInt64 alloc4KPages(UInt64 amount);
        UInt64 alloc4KPages(UInt64             amount,
                            Mem::AddressSpace* addressSpace,
                            Mem::Page4K**      pageReference);
        Void   free4KPages(UInt64 address, UInt64 amount);

        UInt64 alloc(UInt64 size);
        Void   free(UInt64 address);

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