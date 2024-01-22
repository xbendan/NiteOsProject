#include <siberix/mem/mem.h>

namespace Kern::Svc {
    using namespace Mem;

    class MemSvcHost : Svc::ISvcHost
    {
    public:
        MemSvcHost()
          : ISvcHost("KERN.MEM", nullptr)
        {
        }

        uint64_t alloc4KPages(uint64_t amount);
        uint64_t alloc4KPages(uint64_t amount, Mem::AddressSpace* addressSpace);
        void     free4KPages(void* address, uint64_t amount);

        IMemAlloc*    getKernMemAlloc() const;
        IMemAlloc*    getPageMemAlloc() const;
        AddressSpace* getKernelAddressSpace() const;

        // Std::String<Utf8>  getSvcName() override { return "kern.mem\0"; }
        // Std::String<Utf8>* getSvcDepends() override { return nullptr; }
        void onLoad() override;
        void onEnable() override;
        void onDisable() override;

    private:
        IMemAlloc*    m_kmalloc;
        IMemAlloc*    m_physAlloc;
        AddressSpace* m_addressSpaceKern;
        uint64_t      m_totalSize;
        uint64_t      m_usableSize;
    };
}