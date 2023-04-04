#include <Drivers/Storage/AHCI.h>
#include <Fs/Disk.h>
#include <Fs/VirtualFs.h>
#include <Mem/MMIO.h>
#include <Mem/Page.h>

#define HBA_PxIS_TFES (1 << 30)

namespace AHCI
{
    SATADiskDevice::SATADiskDevice(int port, HBAPortRegisters *portRegs, AHCIControllerDevice *controllerDevice)
      : m_MemoryRegs(controllerDevice->MemoryRegisters()),
        m_PortRegs(portRegs),
        DiskDevice("Unidentified SATA Hard Disk Drive")
    {
        StopCommand();

        portRegs->CLB = (controllerDevice->m_clbPhys & 0xFFFFFFFF) + (port << 10);
        portRegs->CLBU = (controllerDevice->m_clbPhys >> 32);

        portRegs->FB = (controllerDevice->m_fbPhys & 0xFFFFFFFF) + (port << 8);
        portRegs->FBU = (controllerDevice->m_fbPhys >> 32);

        m_CommandList = reinterpret_cast<HBACommandHeader *>(Memory::GetIOMapping(portRegs->CLB));

        for (int i = 0; i < 32; i++)
        {
            m_CommandList[i].PRDTL = 8;

            uint64_t phys = controllerDevice->m_ctbaPhys + (port << 13) + (i << 8);
            m_CommandList[i].CTBA = (phys & 0xFFFFFFFF);
            m_CommandList[i].CTBAU = (phys >> 32);
            memset((void *) m_CommandList[i].CTBA, 0, 256);
            
            m_CommandTable[i] = reinterpret_cast<HBACommandTable *>(Memory::GetIOMapping(phys));
        }

        StartCommand();
    }

    SATADiskDevice::~SATADiskDevice()
    {

    }

    void SATADiskDevice::Enable()
    {

    }

    void SATADiskDevice::Disable()
    {

    }

    int SATADiskDevice::Read(uint64_t lba, uint32_t count, void *buffer)
    {
        return Access(lba, count, (uintptr_t) buffer, false);
    }

    int SATADiskDevice::Write(uint64_t lba, uint32_t count, void *buffer)
    {
        return Access(lba, count, (uintptr_t) buffer, true);
    }

    int SATADiskDevice::Access(uint64_t lba, uint32_t count, uintptr_t physBuffer, int write)
    {
        m_PortRegs->IE = 0xFFFFFFFF;
        m_PortRegs->IS = 0;
        int spin = 0;
        int slot = FindCommandSlot();

        if (slot == -1)
            return false;

        m_PortRegs->SERR = m_PortRegs->TFD = 0;

        HBACommandHeader *header = &m_CommandList[slot];

        header->CFL = sizeof(FisHostToDevice) / sizeof(uint32_t);
        header->A = 0;
        header->W = write;
        header->C = 0;
        header->P = 0;

        header->PRDBC = 0;
        header->PMP = 0;

        HBACommandTable *table = m_CommandTable[slot];
        memset(table, 0, sizeof(HBACommandTable));

        table->PRDT[0].DBA = physBuffer & 0xFFFFFFFF;
        table->PRDT[0].DBAU = (physBuffer >> 32) & 0xFFFFFFFF;
        table->PRDT[0].DBC = 512 * count - 1;
        table->PRDT[0].i = 1;

        FisHostToDevice *fis = reinterpret_cast<FisHostToDevice *>(table->CFIS);
        memset(fis, 0, sizeof(FisHostToDevice));

        fis->FisType = FisTypeRegH2D;
        fis->Mode = 1;
        fis->PortMultiplier = 0;

        fis->Command = write ? ATA_CMD_WRITE_DMA_EX : ATA_CMD_READ_DMA_EX;

        fis->LBA0 = lba & 0xFF;
        fis->LBA1 = (lba >> 8) & 0xFF;
        fis->LBA2 = (lba >> 16) & 0xFF;
        fis->Device = 1 << 6;

        fis->LBA3 = (lba >> 24) & 0xFF;
        fis->LBA4 = (lba >> 32) & 0xFF;
        fis->LBA5 = (lba >> 40) & 0xFF;

        fis->CountLow = count & 0xFF;
        fis->CountHigh = count >> 8;

        fis->Control = 0x8;

        while ((m_PortRegs->TFD & (ATA_DEV_BUSY | ATA_DEV_DRQ)) && spin < 1000000)
            spin++;
        if (spin == 1000000)
        {
            return false;
        }

        m_PortRegs->CI = 1 << slot;
        while (true)
        {
            if (!(m_PortRegs->CI & (1 << slot)))
                break;
            
            if (m_PortRegs->IS & HBA_PxIS_TFES)
            {
                //
                return false;
            }
        }

        if (m_PortRegs->IS & HBA_PxIS_TFES)
        {
            return false;
        }
        return true;
    }

    void SATADiskDevice::StartCommand()
    {
        while (m_PortRegs->CMD & HBA_PxCMD_CR);
        m_PortRegs->CMD |= (HBA_PxCMD_FRE | HBA_PxCMD_ST);
    }

    int SATADiskDevice::FindCommandSlot()
    {
        uint32_t slots = (m_PortRegs->SACT | m_PortRegs->CI);
        for (int i = 0; i < 32; i++)
        {
            if (!(slots & 1))
            {
                return i;
            }

            slots >> 1;
        }
        return -1;
    }

    void SATADiskDevice::StopCommand()
    {
        m_PortRegs->CMD &= ~(HBA_PxCMD_ST | HBA_PxCMD_FRE);
        while ((m_PortRegs->CMD & HBA_PxCMD_FR) || (m_PortRegs->CMD & HBA_PxCMD_CR));
    }
}