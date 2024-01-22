#include <drivers/storage/ahci/device.h>
#include <siberix/main.h>

namespace Kern::Hal::Impls {
    AHCIControllerDevice::AHCIControllerDevice(PCIInfo& info)
      : PCIDevice(info, "AHCI Controller Device", DeviceType::DiskController)
      , m_addrBase(getBaseAddrRegs(5))
      , m_clbPhys(Main::alloc4KPages(8))
      , m_fbPhys(Main::alloc4KPages(2))
      , m_ctbaPhys(Main::alloc4KPages(64))
    {
        enableBusMastering();
        enableInterrupts();
        enableMemorySpace();

        m_addrVirt = Mem::copyAsIoAddress(m_addrBase);
        m_memRegs  = reinterpret_cast<HbaMemRegs*>(m_addrVirt);

        m_memRegs->_ghc             &= ~AHCI_GHC_IE;
        m_memRegs->_interruptStatus  = 0xFFFFFFFF;

        for (int i = 0; i < 32; i++) {
            if (!(m_memRegs->_portsImplemented & (1 << i))) {
                continue;
            }

            HbaPortRegs* portRegs = &m_memRegs->_ports[i];
            uint32_t     ssts     = portRegs->_ssts;
            uint8_t      ipm = (ssts >> 8) & 0x0F, det = ssts & 0x0F;
            if (ipm != HBA_PxSSTS_DET_PRESENT || det != HBA_PORT_IPM_ACTIVE) {
                continue;
            }

            switch (portRegs->_sig) {
                case SATA_SIG_ATAPI: {
                    break;
                }
                case SATA_SIG_SEMB: {
                    break;
                }
                case SATA_SIG_PM: {
                    break;
                }
                default: {
                    SataDiskDevice* disk =
                      new SataDiskDevice(i, portRegs, this);

                    if (disk->status() != 1) {
                        // Initializing failed, delete the disk
                        delete disk;
                    } else
                        m_diskList.add(disk);
                    break;
                }
            }
        }
    }
}