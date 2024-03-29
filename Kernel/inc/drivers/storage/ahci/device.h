#include <drivers/pci/device.h>
#include <drivers/storage/ahci/spec.h>
#include <siberix/dvc/type-storage.h>

namespace Kern::Hal::Impls {
    using namespace Specs;

    class AHCIControllerDevice;

    class SataDiskDevice : public DiskDevice
    {
    public:
        SataDiskDevice(int                   port,
                       HBAPortRegs*          portRegs,
                       AHCIControllerDevice* controller);
        ~SataDiskDevice() override = default;

        int64_t read(uint64_t offset, uint64_t size, void* buffer) override;
        int64_t write(uint64_t offset, uint64_t size, void* buffer) override;

        void startCommand();
        void stopCommand();
        int  status() { return m_status; }
        int  access(uint64_t lba,
                    uint32_t count,
                    uint64_t physBuffer,
                    int      write);

        int findSlot();

    private:
        HBAPortRegs*      m_portRegs;
        HBAMemRegs*       m_memRegs;
        HBACommandHeader* m_commandList;
        HBACommandTable*  m_commandTable[8];
        HBAFrameInfo*     m_fis;
        int               m_status;

        uint64_t m_physBuffer[8];
    };

    class AHCIControllerDevice : public PCIDevice
    {
    public:
        AHCIControllerDevice(PCIInfo& info);
        ~AHCIControllerDevice();

        HBAMemRegs* memRegs() { return m_memRegs; }

        uint64_t clbPhys() { return m_clbPhys; }
        uint64_t fbPhys() { return m_fbPhys; }
        uint64_t ctbaPhys() { return m_ctbaPhys; }

    private:
        uint64_t    m_clbPhys, m_fbPhys, m_ctbaPhys;
        uint64_t    m_addrBase;
        uint64_t    m_addrVirt;
        HBAMemRegs* m_memRegs;

        Std::LinkedList<SataDiskDevice> m_diskList;
    };
}
