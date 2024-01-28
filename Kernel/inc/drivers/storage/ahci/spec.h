#include <stdcxx/types.h>

#define AHCI_GHC_HR (1 << 0) // HBA Reset
#define AHCI_GHC_IE (1 << 1) // Interrupt enable
#define AHCI_GHC_ENABLE (1 << 31)

#define AHCI_CAP_S64A (1 << 31) // 64-bit addressing
#define AHCI_CAP_NCQ (1 << 30)  // Support for Native Command Queueing?
#define AHCI_CAP_SSS (1 << 27)  // Supports staggered Spin-up?
#define AHCI_CAP_FBSS (1 << 16) // FIS-based switching supported?
#define AHCI_CAP_SSC (1 << 14)  // Slumber state capable?
#define AHCI_CAP_PSC (1 << 13)  // Partial state capable
#define AHCI_CAP_SALP (1 << 26) // Supports aggressive link power management

#define AHCI_CAP2_NVMHCI (1 << 1) // NVMHCI Present
#define AHCI_CAP2_BOHC (1 << 0)   // BIOS/OS Handoff

#define AHCI_BOHC_BIOS_BUSY (1 << 4)    // BIOS Busy
#define AHCI_BOHC_OS_OWNERSHIP (1 << 3) // OS Ownership Change

#define ATA_CMD_READ_DMA_EX 0x25
#define ATA_CMD_WRITE_DMA_EX 0x35
#define ATA_CMD_IDENTIFY 0xec
#define ATA_DEV_BUSY 0x80
#define ATA_DEV_DRQ 0x08

#define SATA_SIG_SATA 0x00000101  // SATA drive
#define SATA_SIG_ATAPI 0xEB140101 // SATAPI drive
#define SATA_SIG_SEMB 0xC33C0101  // Enclosure management bridge
#define SATA_SIG_PM 0x96690101    // Port multiplier

#define HBA_PxCMD_ST 0x0001
#define HBA_PxCMD_SUD 0x0002
#define HBA_PxCMD_POD 0x0004
#define HBA_PxCMD_FRE 0x0010
#define HBA_PxCMD_FR 0x4000
#define HBA_PxCMD_CR 0x8000
#define HBA_PxCMD_ASP 0x4000000 // Aggressive Slumber/Partial
#define HBA_PxCMD_ICC (0xf << 28)
#define HBA_PxCMD_ICC_ACTIVE (1 << 28)

#define HBA_PxIS_TFES (1 << 30)

#define HBA_PORT_IPM_ACTIVE 1

#define HBA_PxSSTS_DET 0xfULL
#define HBA_PxSSTS_DET_INIT 1
#define HBA_PxSSTS_DET_PRESENT 3

namespace Kern::Hal::Specs {
    enum FisRegs : uint8_t
    {
        TYPE_REG_H2D   = 0x27,
        TYPE_REG_D2H   = 0x34,
        TYPE_DMA_ACT   = 0x39,
        TYPE_DMA_SETUP = 0x41,
        TYPE_DATA      = 0x46,
        TYPE_BIST      = 0x58,
        TYPE_PIO_SETUP = 0x5F,
        TYPE_DEV_BITS  = 0xA1,
    };

    struct RegHostToDevice
    {
        uint8_t _fisType;

        uint8_t _portMultiplier : 4;
        uint8_t _reserved0 : 3;
        uint8_t _mode : 1;

        uint8_t _command;
        uint8_t _featLow;

        union
        {
            dword _dw0;
            struct
            {
                uint8_t _lba0;
                uint8_t _lba1;
                uint8_t _lba2;
                uint8_t _device;
            };
        };

        union
        {
            dword _dw1;
            struct
            {
                uint8_t _lba3;
                uint8_t _lba4;
                uint8_t _lba5;
                uint8_t _featHigh;
            };
        };

        union
        {
            dword _dw2;
            struct
            {
                uint8_t _countLow;
                uint8_t _countHigh;
                uint8_t _icc;
                uint8_t _ctl;
            };
        };

        union
        {
            dword _dw3;
            struct
            {
                uint8_t _reserved1[4];
            };
        };
    } __attribute__((packed));

    struct RegDeviceToHost
    {
        uint8_t _fisType;

        uint8_t _portMultiplier : 4;
        uint8_t _reserved0 : 2;
        uint8_t _interrupt : 1;
        uint8_t _reserved1 : 1;

        uint8_t _statusRegister;
        uint8_t _errorRegister;

        union
        {
            dword _dw0;
            struct
            {
                uint8_t _lba0;
                uint8_t _lba1;
                uint8_t _lba2;
                uint8_t _device;
            };
        };

        union
        {
            dword _dw1;
            struct
            {
                uint8_t _lba3;
                uint8_t _lba4;
                uint8_t _lba5;
                uint8_t _reserved2;
            };
        };

        union
        {
            dword _dw2;
            struct
            {
                uint8_t _countLow;
                uint8_t _countHigh;
                uint8_t _reserved3[2];
            };
        };

        union
        {
            dword _dw3;
            struct
            {
                uint8_t _reserved4[4];
            };
        };
    } __attribute__((packed));

    struct Data
    {
        uint8_t _fisType;

        uint8_t _portMultiplier : 4;
        uint8_t _reserved0 : 4;

        uint8_t _reserved1[2];

        uint32_t _data[1];
    } __attribute__((packed));

    struct PioSetup
    {
        uint8_t _fisType;

        uint8_t _portMultiplier : 4;
        uint8_t _reserved0 : 1;
        uint8_t _interrupt : 1;
        uint8_t _reserved1 : 1;
        uint8_t _status : 1;

        uint8_t _error;

        union
        {
            dword _dw0;
            struct
            {
                uint8_t _lba0;
                uint8_t _lba1;
                uint8_t _lba2;
                uint8_t _device;
            };
        };

        union
        {
            dword _dw1;
            struct
            {
                uint8_t _lba3;
                uint8_t _lba4;
                uint8_t _lba5;
                uint8_t _reserved2;
            };
        };

        union
        {
            dword _dw2;
            struct
            {
                uint8_t _countLow;
                uint8_t _countHigh;
                uint8_t _reserved3;
                uint8_t _e_status;
            };
        };

        union
        {
            dword _dw3;
            struct
            {
                uint16_t _transferCount;
                uint8_t  _reserved4[2];
            };
        };
    } __attribute__((packed));

    struct DmaSetup
    {
        uint8_t _fisType;

        uint8_t _portMultiplier : 4;
        uint8_t _reserved0 : 1;
        uint8_t _interrupt : 1;
        uint8_t _autoActivate : 1;
        uint8_t _reserved1 : 1;

        uint8_t _reserved2[2];

        uint64_t _dmaBufferId;

        uint32_t _reserved3;

        uint32_t _dmaBufferOffset;

        uint32_t _transferCount;

        uint32_t _reserved4;
    } __attribute__((packed));

    struct _HBAPortRegs
    {
        uint32_t _clb;
        uint32_t _clbu;
        uint32_t _fb;
        uint32_t _fbu;
        uint32_t _is;
        uint32_t _ie;
        uint32_t _cmd;
        uint32_t _reserved0;
        uint32_t _tfd;
        uint32_t _sig;
        uint32_t _ssts;
        uint32_t _sctl;
        uint32_t _serr;
        uint32_t _sact;
        uint32_t _ci;
        uint32_t _sntf;
        uint32_t _fbs;
        uint32_t _reserved1[11];
        uint32_t _vendor[4];
    } __attribute__((packed));

    using HBAPortRegs = volatile _HBAPortRegs;

    struct _HBAMemRegs
    {
        uint32_t     _hostCapability;
        uint32_t     _ghc;
        uint32_t     _interruptStatus;
        uint32_t     _portsImplemented;
        uint32_t     _version;
        uint32_t     _ccc_ctl;
        uint32_t     _ccc_pts;
        uint32_t     _em_loc;
        uint32_t     _em_ctl;
        uint32_t     _hostCapabilityExt;
        uint32_t     _bohc;
        uint8_t      _reserved0[0x74];
        uint8_t      _vendor[0x60];
        _HBAPortRegs _ports[0x20];
    } __attribute__((packed));

    using HBAMemRegs = volatile _HBAMemRegs;

    struct _HBAFrameInfo
    {
        DmaSetup _dmaSetup;
        uint8_t  _reserved0[4];

        PioSetup _pioSetup;
        uint8_t  _reserved1[12];

        RegDeviceToHost _d2hRegister;
        uint8_t         _reserved2[4];

        uint8_t devBits[8];

        uint8_t ufis[64];

        uint8_t reserved3[0x60];
    } __attribute__((packed));

    using HBAFrameInfo = volatile _HBAFrameInfo;

    struct _HBACommandHeader
    {
        uint8_t _cfl : 5;
        uint8_t _a : 1;
        uint8_t _w : 1;
        uint8_t _p : 1;

        uint8_t _r : 1;
        uint8_t _b : 1;
        uint8_t _c : 1;
        uint8_t _reserved0 : 1;
        uint8_t _pmp : 4;

        uint16_t _prdtl;

        volatile uint32_t _prdbc;

        uint32_t _ctba;

        uint32_t _ctbau;

        uint32_t _reserved1[4];
    } __attribute__((packed));

    using HBACommandHeader = volatile _HBACommandHeader;

    struct HbaPrdtEntry
    {
        uint32_t _dba;

        uint32_t _dbau;

        uint32_t _reserved0;

        uint32_t _dbc : 22;
        uint32_t _reserved1 : 9;
        uint32_t _i : 1;
    } __attribute__((packed));

    struct _HBACommandTable
    {
        uint8_t _cfis[64];

        uint8_t _acmd[16];

        uint8_t _reserved0[48];

        HbaPrdtEntry _prdtEntry[1];
    } __attribute__((packed));

    using HBACommandTable = volatile _HBACommandTable;

}