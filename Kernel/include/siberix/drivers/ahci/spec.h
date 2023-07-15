#include <common/typedefs.h>

#define AHCI_BASE 0x400000

#define AHCI_GHC_HR (1 << 0)  // HBA Reset
#define AHCI_GHC_IE (1 << 1)  // Interrupt enable
#define AHCI_GHC_ENABLE (1 << 31)

#define AHCI_CAP_S64A (1 << 31)  // 64-bit addressing
#define AHCI_CAP_NCQ (1 << 30)   // Support for Native Command Queueing?
#define AHCI_CAP_SSS (1 << 27)   // Supports staggered Spin-up?
#define AHCI_CAP_FBSS (1 << 16)  // FIS-based switching supported?
#define AHCI_CAP_SSC (1 << 14)   // Slumber state capable?
#define AHCI_CAP_PSC (1 << 13)   // Partial state capable
#define AHCI_CAP_SALP (1 << 26)  // Supports aggressive link power management

#define AHCI_CAP2_NVMHCI (1 << 1)  // NVMHCI Present
#define AHCI_CAP2_BOHC (1 << 0)    // BIOS/OS Handoff

#define AHCI_BOHC_BIOS_BUSY (1 << 4)     // BIOS Busy
#define AHCI_BOHC_OS_OWNERSHIP (1 << 3)  // OS Ownership Change

#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA_EX 0x35
#define ATA_CMD_IDENTIFY 0xec
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define SATA_SIG_SATA 0x00000101   // SATA drive
#define SATA_SIG_ATAPI 0xEB140101  // SATAPI drive
#define SATA_SIG_SEMB 0xC33C0101   // Enclosure management bridge
#define SATA_SIG_PM 0x96690101     // Port multiplier

#define AHCI_DEV_NULL 0
#define AHCI_DEV_SATA 1
#define AHCI_DEV_SEMB 2
#define AHCI_DEV_PM 3
#define AHCI_DEV_SATAPI 4

#define HBA_PORT_IPM_ACTIVE 1
#define HBA_PORT_DET_PRESENT 3

#define HBA_PxCMD_ST 0x0001
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_FR 0x4000
#define HBA_PxCMD_CR 0x8000

enum class FisRegisters {
    H2D        = 0x27,  // Register FIS - host to device
    D2H        = 0x34,  // Register FIS - device to host
    DMAAct     = 0x39,  // DMA activate FIS - device to host
    DMASetup   = 0x41,  // DMA setup FIS - bidirectional
    Data       = 0x46,  // Data FIS - bidirectional
    BIST       = 0x58,  // BIST activate FIS - bidirectional
    PIOSetup   = 0x5f,  // PIO setup FIS - device to host
    DeviceBits = 0xa1   // Set device bits FIS - device to host
};

struct FisHostToDevice {
    // DWORD 0
    u8 fisType;

    u8 portMultiplier : 4;
    u8 __reserved__0 : 3;
    u8 mode : 1;

    u8 command;
    u8 featureLow;

    // DWORD 1
    u8 lba0, lba1, lba2;
    u8 device;

    // DWORD 2
    u8 lba3, lba4, lba5;
    u8 featureHigh;

    // DWORD 3
    u8 countLow;
    u8 countHigh;
    u8 icc;  // Isochronous Command Completion
    u8 ctrl;

    // DWORD 4
    u8 __reserved__1[4];
};

struct FisDeviceToHost {
    // DWORD 0
    u8 fisType;

    u8 portMultiplier : 4;
    u8 __reserved__0 : 2;
    u8 interrupt : 1;
    u8 __reserved__1 : 1;

    u8 status;
    u8 error;

    // DWORD 1
    u8 lba0, lba1, lba2;
    u8 device;

    // DWORD 2
    u8 lba3, lba4, lba5;
    u8 __reserved__2;

    // DWORD 3
    u8 countLow;
    u8 countHigh;
    u8 __reserved__3[2];

    // DWROD 4
    u8 __reserved__4[4];
};

struct FisData {
    u8 fisType;

    u8 portMultiplier : 4;
    u8 __reserved__0 : 4;

    u8 __reserved__1[2];

    u32 data[1];  // payload
};

struct FisSetupPio {
    // DWORD 0
    u8 fisType;

    u8 portMultiplier : 4;
    u8 __reserved__0 : 1;
    u8 direction : 1;
    u8 interrupt : 1;
    u8 __reserved__1 : 1;

    u8 status;
    u8 error;

    // DWORD 1
    u8 lba0, lba1, lba2;
    u8 device;

    // DWORD 2
    u8 lba3, lba4, lba5;
    u8 __reserved__2;

    // DWORD 3
    u8 countLow;
    u8 countHigh;
    u8 __reserved__3;
    u8 newStatus;

    // DWORD 4
    u16 transferCount;
    u8  __reserved__4[2];
};

struct FisSetupDma {
    u8 fisType;

    u8 portMultiplier : 4;
    u8 __reserved__0 : 1;
    u8 direction : 1;
    u8 interrupt : 1;
    u8 autoActivate : 1;

    u8 __reserved__1[2];

    /**
     * @brief DMA Buffer Identifier.
     *
     * Used to Identify DMA buffer in host memory.
     * SATA Spec says host specific and not in Spec. Trying AHCI spec might work
     */
    u64 bufferId;

    u32 __reserved__2;

    u32 bufferOffset;
    u32 transferCount;

    u32 __reserved__3;
};

volatile struct HbaPortRegisters {
    u32 clb, clbu, fb, fbu, is, ie, cmd;
    u32 __reserved__0;
    u32 tfd, sig, ssts, sctl, serr, sact, ci, sntf, fbs;
    u32 __reserved__1[11];
    u32 vendor[4];
};

volatile struct HbaMemoryRegisters {
    u32 hostCapability;
    u32 globalHostControl;
    u32 interruptStatus;
    u32 portImplemented;
    u32 version;
    u32 cccControl;
    u32 cccPorts;
    u32 emLocation;
    u32 emControl;
    u32 hostCapabilityExt;
    u32 bohc;

    u8 __reserved__[0xa0 - 0x2c];
    u8 vendor[0x100 - 0xa0];

    HbaPortRegisters ports[];
};

volatile struct HbaFrameInfo {
    FisSetupDma setupDMA;
    u8          __padding__0[4];

    FisSetupPio setupPIO;
    u8          __padding__1[12];

    FisDeviceToHost d2h;
    u8              __padding__2[4];

    u8 setBits[8];
    u8 ufis[64];
    u8 __reserved__[0x100 - 0xa0];
};

struct HbaCommandHeader {
    // DWORD 0
    uint8_t cfl : 5;  // Command FIS length in DWORDS, 2 ~ 16
    uint8_t a : 1;    // ATAPI
    uint8_t w : 1;    // Write, 1: H2D, 0: D2H
    uint8_t p : 1;    // Prefetchable

    uint8_t r : 1;              // Reset
    uint8_t b : 1;              // BIST
    uint8_t c : 1;              // Clear busy upon R_OK
    uint8_t __reserved__0 : 1;  // Reserved
    uint8_t pmp : 4;            // Port multiplier port

    uint16_t prdtl;  // Physical region descriptor table length in entries

    // DWORD 1
    volatile uint32_t
        prdbc;  // Physical region descriptor byte count transferred

    // DWORD 2, 3
    uint32_t ctba;   // Command table descriptor base address
    uint32_t ctbau;  // Command table descriptor base address upper 32 bits

    // DWORD 4, 5, 6, 7
    uint32_t __reserved__1[4];  // Reserved
};

struct HbaPhysicalRegionEntry {
    u32 dba;
    u32 dbau;
    u32 __reserved__0;

    u32 dbc : 22;
    u32 __reserved__1 : 9;
    u32 i : 1;
};

struct HbaCommandTable {
    u8                     cfis[64];
    u8                     acmd[16];
    u8                     __reserved__[48];
    HbaPhysicalRegionEntry prdt[1];
};
