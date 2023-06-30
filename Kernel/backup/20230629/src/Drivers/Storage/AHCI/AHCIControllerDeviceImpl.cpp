#include <Drivers/Storage/AHCI.h>
#include <Drivers/Bus/PCI.h>
#include <Mem/MMIO.h>
#include <Mem/Page.h>

namespace AHCI
{
    AHCIControllerDevice::AHCIControllerDevice(PCI::PCIInfo deviceInfo)
      : PCI::PCIDevice(deviceInfo),
        m_AddrBase(this->GetBaseAddressRegister(5)),
        m_clbPhys(Memory::KernelAllocate4KPages(8)),
        m_fbPhys(Memory::KernelAllocate4KPages(2)),
        m_ctbaPhys(Memory::KernelAllocate4KPages(64))
    {
        m_Name = "AHCI Controller";
        m_Type = DeviceType::DeviceTypeDiskController;
        memset((void *) m_clbPhys, 0, 8 * ARCH_PAGE_SIZE);
        memset((void *) m_fbPhys, 0, 2 * ARCH_PAGE_SIZE);

        EnableBusMastering();
        EnableInterrupts();
        EnableMemorySpace();

        Objects::Validate(m_AddrBase != 0x0, "Cannot read AHCI controller base.");

        m_AddrVirt = Memory::GetIOMapping(m_AddrBase);
        m_HBA = reinterpret_cast<HBAMemoryRegisters *>(m_AddrVirt);

        m_HBA->GlobalHostControl &= ~AHCI_GHC_IE;
        m_HBA->InterruptStatus = 0xFFFFFFFF;

        auto typeOf = [](HBAPortRegisters *portRegs) -> int
        {
            uint32_t ssts = portRegs->SSTS;
            uint8_t ipm = (ssts >> 8) & 0x0F, det = ssts & 0x0F;

            if (ipm != HBA_PORT_DET_PRESENT ||
                det != HBA_PORT_IPM_ACTIVE)
            {
                return AHCI_DEV_NULL;
            }

            switch (portRegs->SIG)
            {
                case SATA_SIG_ATAPI: { return AHCI_DEV_SATAPI; }
                case SATA_SIG_SEMB: { return AHCI_DEV_SEMB; }
                case SATA_SIG_PM: { return AHCI_DEV_PM; }
                default: { return AHCI_DEV_SATA; }
            }
        };

        for (int i = 0; i < 32; i++)
        {
            if (!(m_HBA->PortImplemented & (1 << i)))
                continue;

            HBAPortRegisters *portRegs = &m_HBA->Ports[i];
            switch (typeOf(portRegs))
            {
                case AHCI_DEV_SATA: {
                    SATADiskDevice *disk = new SATADiskDevice(i, portRegs, this);
                    
                    if (disk->Status() != 1)
                    {
                        // Initialized failed.
                        delete disk;
                    } else m_DiskList.Add(disk);
                }
            }
        }
    }
    
    void AHCIControllerDevice::Enable()
    {

    }

    void AHCIControllerDevice::Disable()
    {
        
    }
} // namespace AHCI
