#include <drivers/storage/ahci/device.h>
#include <siberix/main.h>
#include <siberix/mem/range.h>

namespace Kern::Hal::Impls {
    SataDiskDevice::SataDiskDevice(int                   port,
                                   HbaPortRegs*          portRegs,
                                   AHCIControllerDevice* controller)
      : DiskDevice("Unidentified SATA Hard Disk Drive")
      , m_portRegs(portRegs)
      , m_memRegs(controller->memRegs())
    {
        stopCommand();

        portRegs->_clb  = (controller->clbPhys() & 0xFFFFFFFF) + (port << 10);
        portRegs->_clbu = (controller->clbPhys() >> 32);

        portRegs->_fb  = (controller->fbPhys() & 0xFFFFFFFF) + (port << 8);
        portRegs->_fbu = (controller->fbPhys() >> 32);

        m_commandList = reinterpret_cast<HbaCommandHeader*>(
          Mem::copyAsIoAddress(portRegs->_clb));

        for (int i = 0; i < 32; i++) {
            m_commandList[i]._prdtl = 8;

            uint64_t phys = controller->ctbaPhys() + (port << 13) + (i << 8);
            m_commandList[i]._ctba  = (phys & 0xFFFFFFFF);
            m_commandList[i]._ctbau = (phys >> 32);
            Mem::rangeOf((uint64_t)(m_commandList[i]._ctba), 256).clear();

            m_commandTable[i] =
              reinterpret_cast<HbaCommandTable*>(Mem::copyAsIoAddress(phys));
        }

        startCommand();
    }

    SataDiskDevice::~SataDiskDevice() {}

    int64_t SataDiskDevice::read(uint64_t offset, uint64_t size, void* buffer)
    {
        return access(offset / 512, size / 512, (uint64_t)buffer, false);
    }

    int64_t SataDiskDevice::write(uint64_t offset, uint64_t size, void* buffer)
    {
        return access(offset / 512, size / 512, (uint64_t)buffer, true);
    }

    void SataDiskDevice::startCommand()
    {
        while (m_portRegs->_cmd & HBA_PxCMD_CR)
            ;
        m_portRegs->_cmd |= (HBA_PxCMD_FRE | HBA_PxCMD_ST);
    }

    void SataDiskDevice::stopCommand()
    {
        m_portRegs->_cmd &= ~(HBA_PxCMD_FRE | HBA_PxCMD_ST);
        while ((m_portRegs->_cmd & HBA_PxCMD_FR) ||
               (m_portRegs->_cmd & HBA_PxCMD_CR))
            ;
    }

    int SataDiskDevice::access(uint64_t lba,
                               uint32_t count,
                               uint64_t physBuf,
                               int      write)
    {
        m_portRegs->_ie = 0xffffffff;
        m_portRegs->_is = 0;
        int spin = 0, slot = findSlot();

        if (slot == -1)
            return false;

        m_portRegs->_serr = m_portRegs->_tfd = 0;

        HbaCommandHeader* header = &m_commandList[slot];
        header->_cfl             = sizeof(RegHostToDevice) / sizeof(uint32_t);
        header->_a               = 0;
        header->_w               = write;
        header->_c               = 0;
        header->_p               = 0;
        // header->_prdtl = (uint16_t)((count - 1) >> 4) + 1;
        header->_prdbc           = 0;
        header->_pmp             = 0;

        HbaCommandTable* table = m_commandTable[slot];
        Mem::rangeOf((uint64_t)table, sizeof(HbaCommandTable)).clear();
        table->_prdtEntry[0]._dba  = physBuf & 0xFFFFFFFF;
        table->_prdtEntry[0]._dbau = (physBuf >> 32) & 0xFFFFFFFF;
        table->_prdtEntry[0]._dbc  = (count << 9) - 1;
        table->_prdtEntry[0]._i    = 1;

        RegHostToDevice* h2d = (RegHostToDevice*)table->_cfis;
        Mem::rangeOf((uint64_t)h2d, sizeof(RegHostToDevice)).clear();
        h2d->_fisType        = FisRegs::TYPE_REG_H2D;
        h2d->_mode           = 1;
        h2d->_portMultiplier = 0;
        h2d->_command   = write ? ATA_CMD_WRITE_DMA_EX : ATA_CMD_READ_DMA_EX;
        h2d->_lba0      = (uint8_t)(lba & 0xFF);
        h2d->_lba1      = (uint8_t)((lba >> 8) & 0xFF);
        h2d->_lba2      = (uint8_t)((lba >> 16) & 0xFF);
        h2d->_lba3      = (uint8_t)((lba >> 24) & 0xFF);
        h2d->_lba4      = (uint8_t)((lba >> 32) & 0xFF);
        h2d->_lba5      = (uint8_t)((lba >> 40) & 0xFF);
        h2d->_device    = 1 << 6;
        h2d->_countLow  = count & 0xFF;
        h2d->_countHigh = (count >> 8) & 0xFF;
        h2d->_ctl       = 0x8;

        while ((m_portRegs->_tfd & (ATA_DEV_BUSY | ATA_DEV_DRQ)) &&
               spin < 1000000)
            spin++;
        if (spin == 1000000) {
            return false;
        }

        m_portRegs->_ci = 1 << slot;
        while (true) {
            if (!(m_portRegs->_ci & (1 << slot)))
                break;

            if (m_portRegs->_is & HBA_PxIS_TFES) {
                //
                return false;
            }
        }

        return !(m_portRegs->_is & HBA_PxIS_TFES);
    }

    int SataDiskDevice::findSlot()
    {
        uint32_t slots = (m_portRegs->_sact | m_portRegs->_ci);
        for (int i = 0; i < 32; i++) {
            if (!(slots & 1)) {
                return i;
            }
            slots >>= 1;
        }
        return -1;
    }
}