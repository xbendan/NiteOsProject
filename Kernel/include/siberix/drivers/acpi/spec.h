#include <common/typedefs.h>

struct AcpiRsdp {
    char signature[8];
    u8   checksum;
    char oemId[6];
    u8   revision;
    u32  rsdtAddress;
} __attribute__((packed));

struct AcpiXsdp : public AcpiRsdp {
    u32 length;
    u64 xsdtAddress;
    u8  checksumEx;
    u8  reserved[3];
};

struct AcpiTable {
    char signature[4];
    u32  length;
    u8   revision;
    u8   checksum;
    char oemId[6];
    char oemTableId[8];
    u32  oemRevision;
    u32  creatorId;
    u32  creatorRevision;
} __attribute__((packed));

struct AcpiRsdt : public AcpiTable {
    u32 pointers[];
} __attribute__((packed));

struct AcpiXsdt : public AcpiTable {
    u64 pointers[];
} __attribute__((packed));

struct AcpiAddress {
    /**
     * @brief address space indicates where to read data
     * 0: System Memory
     * 1: System I/O
     * 2: PCI Configuration Space
     * 3: Embedded Controller
     * 4: @deprecated System Management Bus
     * 5: System CMOS
     * 6: PCI Device BAR Target
     * 7: @deprecated Intelligent Platform Management Infrastructure
     * 8: @deprecated General Purpose I/O
     * 9: @deprecated Generic Serial Bus
     * 0x0a: @deprecated Platform Communication Channel
     * 0x0b-0x7f: reserved
     * 0x80-0xff: OEM defined
     */
    u8  addressSpace;
    u8  bitWidth;
    u8  bitOffset;
    u8  accessSize;
    u64 address;
};

struct MadtEntry {
    u8 type;
    u8 length;
} __attribute__((packed));

struct Madt /* Multiple APIC Description Table */ : public AcpiTable {
    u32       address;
    u32       flags;
    MadtEntry entries;
} __attribute__((packed));

struct MadtLocalApic : public MadtEntry {
    u8  processorId;
    u8  apicId;
    u32 flags;
} __attribute__((packed));

struct MadtIoApic : public MadtEntry {
    u8  apicId;
    u8  __reserved__;
    u32 address;
    u32 gSiB;
} __attribute__((packed));

struct MadtIso : public MadtEntry {
    u8  busSource;
    u8  irqSource;
    u32 gSi;
    u16 flags;
} __attribute__((packed));

struct MadtNmi : public MadtEntry {
    u8  processorId;
    u16 flags;
    u8  lInt;
} __attribute__((packed));

struct MadtLocalx2Apic : public MadtEntry {
    u16 __reserved__;
    u32 x2apicId;
    u32 flags;
    u32 uid;
} __attribute__((packed));

struct MadtNmix2Apic : public MadtEntry {
    u16 flags;
    u32 uid;
    u8  lInt;
    u8  __reserved__[3];
} __attribute__((packed));

struct Hpet : public AcpiTable {
    u8          hwrevId;
    u8          info;
    u16         pciVendorId;
    AcpiAddress address;
    u8          hpetNumber;
    u16         minTick;
    u8          pageProtection;
};

struct McfgAddress {
    u64 base;
    u16 segmentGroupNum;
    u8  busStart;
    u8  busEnd;
    u32 __reserved__;
};

struct PciMcfg : public AcpiTable {
    u64         __reserved__;
    McfgAddress baseAddresses[];
};

struct AcpiFadt /* Fixed ACPI Description Table */ : public AcpiTable {
    u32 fwctrl;
    u32 dsdt;

    u8 __reserved__0;

    u8  ppmp;
    u16 sciInt;
    u32 smiCmd;
    u8  enable;
    u8  disable;
    u8  s4biosReq;
    u8  pStateCtrl;
    u32 pm1aEventBlock;
    u32 pm1bEventBlock;
    u32 pm1aCtrlBlock;
    u32 pm1bCtrlBlock;
    u32 pmtTimerBlock;
    u32 gpe0Block;
    u32 gpe1Block;
    u8  pm1EventLength;
    u8  pm1CtrlLength;
    u8  pm2CtrlLength;
    u8  pmtTimerLength;
    u8  gpe0Length;
    u8  gpe1Length;
    u8  gpe1Base;
    u8  cStateCtrl;
    u16 worstC2Latency;
    u16 worstC3Latency;
    u16 flushSize;
    u16 flushStride;
    u8  dutyOffset;
    u8  dutyWidth;
    u8  dayAlarm;
    u8  monthAlarm;
    u8  century;

    u16 bootFlagIAPC;

    u8 __reserved__1;

    u32 flags;

    AcpiAddress resetRegistry;
    u8          resetValue;
    u16         bootFlagARM;
    u8          minor;

    u64         x_fwctrl;
    u64         x_dsdt;
    AcpiAddress x_pm1aEventBlock;
    AcpiAddress x_pm1bEventBlock;
    AcpiAddress x_pm1aCtrlBlock;
    AcpiAddress x_pm1bCtrlBlock;
    AcpiAddress x_pm2CtrlBlock;
    AcpiAddress x_pmtTimerBlock;
    AcpiAddress x_gpe0Block;
    AcpiAddress x_gpe1Block;
};

struct AcpiFacs /* Firmware ACPI Control Structure */ {};

struct AcpiSbst /* Smart Battery Description Table */ : public AcpiTable {
    u32 warningEnergyLevel;
    u32 lowEnergyLevel;
    u32 criticalEnergyLevel;
};

struct AcpiEcdt /* Embedded Controller Boot Resources Table */
    : public AcpiTable {
    AcpiAddress ecCtrl;
    AcpiAddress ecData;
    u32         uid;
    u8          gpeBit;
    u8          ecId[];
};

struct AcpiDsdt /* Differentiated System Description Table */
    : public AcpiTable {
    u8 aml[];
};

struct AcpiSsdt /* Secondary System Description Table */ : public AcpiTable {
    u8 aml[];
};