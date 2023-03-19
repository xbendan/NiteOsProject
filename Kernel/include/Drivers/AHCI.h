#include <Device/Device.h>
#include <Drivers/PCI.h>
#include <Fs/Disk.h>
#include <Utils/LinkedList.h>
#include <macros>

#define AHCI_BASE       0x400000

#define AHCI_GHC_HR (1 << 0) // HBA Reset
#define AHCI_GHC_IE (1 << 1) // Interrupt enable
#define AHCI_GHC_ENABLE (1 << 31)

#define AHCI_CAP_S64A (1 << 31) // 64-bit addressing
#define AHCI_CAP_NCQ (1 << 30) // Support for Native Command Queueing?
#define AHCI_CAP_SSS (1 << 27) // Supports staggered Spin-up?
#define AHCI_CAP_FBSS (1 << 16) // FIS-based switching supported?
#define AHCI_CAP_SSC (1 << 14) // Slumber state capable?
#define AHCI_CAP_PSC (1 << 13) // Partial state capable
#define AHCI_CAP_SALP (1 << 26) // Supports aggressive link power management

#define AHCI_CAP2_NVMHCI (1 << 1) // NVMHCI Present
#define AHCI_CAP2_BOHC (1 << 0) // BIOS/OS Handoff

#define AHCI_BOHC_BIOS_BUSY (1 << 4) // BIOS Busy
#define AHCI_BOHC_OS_OWNERSHIP (1 << 3) // OS Ownership Change

#define ATA_CMD_READ_DMA_EX     0x25
#define ATA_CMD_WRITE_DMA_EX    0x35
#define ATA_CMD_IDENTIFY        0xec
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define	SATA_SIG_SATA	0x00000101	// SATA drive
#define	SATA_SIG_ATAPI	0xEB140101	// SATAPI drive
#define	SATA_SIG_SEMB	0xC33C0101	// Enclosure management bridge
#define	SATA_SIG_PM	    0x96690101	// Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

#define HBA_PxCMD_ST    0x0001
#define HBA_PxCMD_FRE   0x0010
#define HBA_PxCMD_FR    0x4000
#define HBA_PxCMD_CR    0x8000

namespace AHCI 
{
    enum FisRegisters
    {
        FisTypeRegH2D = 0x27, // Register FIS - host to device
        FisTypeRegD2H = 0x34, // Register FIS - device to host
        FisTypeDMAAct = 0x39, // DMA activate FIS - device to host
        FisTypeDMASetup = 0x41, // DMA setup FIS - bidirectional
        FisTypeData = 0x46, // Data FIS - bidirectional
        FisTypeBIST = 0x58, // BIST activate FIS - bidirectional
        FisTypePIOSetup = 0x5F, // PIO setup FIS - device to host
        FisTypeDeviceBits = 0xA1 // Set device bits FIS - device to host
    };

    struct FisHostToDevice
    {
        // DWORD 0
        uint8_t FisType /* FIS_TYPE_REG_H2D */;

        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 3;
        uint8_t Mode: 1;

        uint8_t Command;
        uint8_t FeatureLow;

        // DWORD 1
        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        // DWORD 2
        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t FeatureHigh;

        // DWORD 3
        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t ICC; // Isochronous Command Completion
        uint8_t Control;

        // DWORD 4
        uint8_t __reserved__1[4];
    };

    struct FisDeviceToHost
    {
        // DWORD 0
        uint8_t FisType /* FIS_TYPE_REG_D2H */;

        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 2;
        uint8_t Interrupt: 1;
        uint8_t __reserved__1: 1;

        uint8_t Status;
        uint8_t Error;

        // DWORD 1
        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        // DWORD 2
        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t __reserved__2;

        // DWORD 3
        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t __reserved__3[2];

        // DWORD 4
        uint8_t __reserved__4[4];
    };

    struct FisData
    {
        uint8_t FisType /* FIS_TYPE_DATA */;
        
        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 4;

        uint8_t __reserved__1[2];

        uint32_t data[1]; // Payload
    };

    struct FisSetupPIO
    {
        // DWORD 0
        uint8_t FisType /* FIS_TYPE_REG_D2H */;

        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 1;
        uint8_t TransferDirection: 1;
        uint8_t Interrupt: 1;
        uint8_t __reserved__1: 1;

        uint8_t Status;
        uint8_t Error;

        // DWORD 1
        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        // DWORD 2
        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t __reserved__2;

        // DWORD 3
        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t __reserved__3;
        uint8_t NewStatus; // New value of status register

        // DWORD 4
        uint16_t TransferCount;
        uint8_t __reserved__4[2];
    };

    struct FisSetupDMA
    {
        uint8_t FisType /* FIS_TYPE_DMA_SETUP */;
        
        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 1;
        uint8_t TransferDirection: 1;
        uint8_t Interrupt: 1;
        uint8_t AutoActivate: 1;

        uint8_t __reserved__1[2];

        
        uint64_t BufferId;  // DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
                            // SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

        uint32_t __reserved__2;

        uint32_t BufferOffset;

        uint32_t TransferCount;

        uint32_t __reserved__3;
    };

    typedef volatile struct HBAPortRegisters
    {
        uint32_t CLB, CLBU, FB, FBU, IS, IE, CMD;
        uint32_t __reserved__0;
        uint32_t TFD, SIG, SSTS, SCTL, SERR, SACT, CI, SNTF, FBS;
        uint32_t __reserved__1[11];
        uint32_t Vendor[4];
    } hba_port_t;

    typedef volatile struct HBAMemoryRegisters
    {
        uint32_t HostCapability;
        uint32_t GlobalHostControl;
        uint32_t InterruptStatus;
        uint32_t PortImplemented;
        uint32_t Version;
        uint32_t CCCControl;
        uint32_t CCCPorts;
        uint32_t EMLocation;
        uint32_t EMControl;
        uint32_t HostCapabilityExt;
        uint32_t BOHC;
        
        uint8_t __reserved__[0xA0 - 0x2C];

        uint8_t Vendor[0x100 - 0xA0];

        HBAPortRegisters Ports[];
    } hba_mem_t;

    typedef volatile struct HBAFrameInformation
    {
        FisSetupDMA SetupDMA;
        uint8_t __padding__0[4];

        FisSetupPIO SetupPIO;
        uint8_t __padding__1[12];

        FisDeviceToHost D2H;
        uint8_t __padding__2[4];

        uint8_t SetBits[8];

        uint8_t ufis[64];
        
        uint8_t __reserved__[0x100 - 0xA0];
    } hba_fis_t;

    struct HBACommandHeader
    {
        // DW0
        uint8_t  CFL:5;		// Command FIS length in DWORDS, 2 ~ 16
        uint8_t  A:1;		// ATAPI
        uint8_t  W:1;		// Write, 1: H2D, 0: D2H
        uint8_t  P:1;		// Prefetchable
    
        uint8_t  R:1;		// Reset
        uint8_t  B:1;		// BIST
        uint8_t  C:1;		// Clear busy upon R_OK
        uint8_t  __reserved__0:1;		// Reserved
        uint8_t  PMP:4;		// Port multiplier port
    
        uint16_t PRDTL;		// Physical region descriptor table length in entries
    
        // DW1
        volatile
        uint32_t PRDBC;		// Physical region descriptor byte count transferred
    
        // DW2, 3
        uint32_t CTBA;		// Command table descriptor base address
        uint32_t CTBAU;		// Command table descriptor base address upper 32 bits
    
        // DW4 - 7
        uint32_t __reserved__1[4];	// Reserved
    };

    struct HBAPhysicalRegionEntry
    {
        uint32_t DBA;
        uint32_t DBAU;
        uint32_t __reserved__0;

        uint32_t DBC: 22;
        uint32_t __reserved__1: 9;
        uint32_t i: 1;
    };
    

    struct HBACommandTable
    {
        uint8_t CFIS[64];
        uint8_t ACMD[16];
        uint8_t __reserved__[48];
        HBAPhysicalRegionEntry PRDT[1];
    };

    class AHCIControllerDevice;

    class SATADiskDevice : public DiskDevice
    {
    private:
        HBAPortRegisters *m_PortRegs;
        HBAMemoryRegisters *m_MemoryRegs;
        int m_Status = 0;

        HBACommandHeader *m_CommandList;
        HBACommandTable *m_CommandTable[8];
        HBAFrameInformation *m_FrameInfos;

    public:
        SATADiskDevice(int port, HBAPortRegisters *portRegs, AHCIControllerDevice *controllerDevice);
        ~SATADiskDevice();

        virtual void Enable();
        virtual void Disable();

        int Access(uint64_t lba, uint32_t count, uintptr_t physBuffer, int write);

        virtual int Read(uint64_t lba, uint32_t count, void *buffer);
        virtual int Write(uint64_t lba, uint32_t count, void *buffer);

        inline int Status() { return m_Status; }
        int FindCommandSlot();

        void StartCommand();
        void StopCommand();
    };

    class AHCIControllerDevice : public PCI::PCIDevice
    {
    private:
        LinkedList<SATADiskDevice *> m_DiskList;
        uint64_t m_AddrBase;
        uint64_t m_AddrVirt;
        HBAMemoryRegisters *m_HBA;

    public:
        uint64_t m_clbPhys, m_fbPhys, m_ctbaPhys;

        AHCIControllerDevice(PCI::PCIInfo deviceInfo);
        ~AHCIControllerDevice();

        inline HBAMemoryRegisters *MemoryRegisters() { return m_HBA; }

        virtual void Enable();
        virtual void Disable();
    };

    void Initialize();
}