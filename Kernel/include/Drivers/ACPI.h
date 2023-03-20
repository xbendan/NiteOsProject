#include <Utils/LinkedList.h>
#include <Utils/ArrayList.h>
#include <Timer.h>
#include <macros>

#include <Arch/x86_64/mmu.h>

enum AcpiTableNameDefinition
{
    AcpiMultiDescriptionTable = 0
};

struct ACPITable
{
    char Signature[4];
    uint32_t Length;
    uint8_t Revision;
    uint8_t Checksum;
    char OemId[6];
    char OemTableId[8];
    uint32_t OemRevision;
    uint32_t CreatorId;
    uint32_t CreatorRevision;
} __attribute__((packed));

typedef struct MADTEntry
{
    uint8_t type;
    uint8_t length;
} __attribute__((packed)) madt_entry_t;

typedef struct MADT : public ACPITable
{
    uint32_t address;
    uint32_t flags;
    madt_entry_t entries[];
} __attribute__((packed)) acpi_madt_t;

typedef struct MadtLocalApic : public MADTEntry
{
    uint8_t processorId;
    uint8_t apicId;
    uint32_t flags;
} __attribute__((packed)) madt_local_t;

typedef struct MadtIoApic : public MADTEntry
{
    uint8_t apicId;
    uint8_t reserved;
    uint32_t address;
    uint32_t gSib;
} __attribute__((packed)) madt_io_t;

typedef struct MadtIso : public MADTEntry
{
    uint8_t busSource;
    uint8_t irqSource;
    uint32_t gSi;
    uint16_t Flags;
} __attribute__((packed)) madt_iso_t;

typedef struct MadtNmi : public MADTEntry
{
    uint8_t ProcessorId;
    uint16_t Flags;
    uint8_t LINT;
} __attribute__((packed)) madt_nmi_t;

typedef struct ACPIAddress
{
    uint8_t AddressSpace;
    uint8_t BitWidth;
    uint8_t BitOffset;
    uint8_t AccessSize;
    uint64_t Address;
} acpi_gas_t;

typedef struct Fadt : public ACPITable
{
    uint32_t FirmwareControl;
    uint32_t DSDT;

    uint8_t __reserved__0;

    uint8_t ppmp;
    uint16_t SCIInterrupt;
    uint32_t SMICommandPort;
    uint8_t Enable;
    uint8_t Disable;
    uint8_t S4BIOSReq;
    uint8_t PStateControl;
    uint32_t PM1aEventBlock;
    uint32_t PM1bEventBlock;
    uint32_t PM1aControlBlock;
    uint32_t PM1bControlBlock;
    uint32_t PM2ControlBlock;
    uint32_t PMTTimerBlock;
    uint32_t GPE0Block;
    uint32_t GPE1Block;
    uint8_t PM1EventLength;
    uint8_t PM1ControlLength;
    uint8_t PM2ControlLength;
    uint8_t PMTTimerLength;
    uint8_t GPE0Length;
    uint8_t GPE1Length;
    uint8_t GPE1Base;
    uint8_t CStateControl;
    uint16_t WorstC2Latency;
    uint16_t WorstC3Latency;
    uint16_t FlushSize;
    uint16_t FlushStride;
    uint8_t DutyOffset;
    uint8_t DutyWidth;
    uint8_t DayAlarm;
    uint8_t MonthAlarm;
    uint8_t Century;

    uint16_t BootArchFlags;
    uint8_t __reserved__1;
    uint32_t Flags;

    ACPIAddress ResetRegistry;

    uint8_t ResetVal;
    uint16_t ARMBootArch;
    uint8_t MinorVersion;

    uint64_t xFirmwareControl;
    uint64_t xDSDT;

    ACPIAddress xPM1aEventBlock;
    ACPIAddress xPM1bEventBlock;
    ACPIAddress xPM1aControlBlock;
    ACPIAddress xPM1bControlBlock;
    ACPIAddress xPM2ControlBlock;
    ACPIAddress xPMTTimerBlock;
    ACPIAddress xGPE0Block;
    ACPIAddress xGPE1Block;
} acpi_fadt_t;

typedef struct MadtAddressOverride : public MADTEntry
{
    uint16_t reserved;
    uint64_t address;
} madt_address_override_t;

struct MCFGAddress
{
    uint64_t base;
    uint16_t SegmentGroupNumber;
    uint8_t pci_bus_start;
    uint8_t pci_bus_end;
    uint32_t __reserved__;
};

struct MCFG : public ACPITable
{
    uint64_t __reserved__;
    MCFGAddress bases[];
};

struct HPET : public ACPITable
{
    uint8_t hardwareRevisionId;
    uint8_t info;
    uint16_t pciVendorId;
    uint8_t addressSpaceId;
    uint8_t registerBitWidth;
    uint8_t registerBitOffset;
    uint8_t reserved;
    uint64_t address;
    uint8_t hpetNumber;
    uint16_t minimumTick;
    uint8_t pageProtection;
};

namespace ACPI
{
    struct RootSystemDescriptionPointer
    {
        char Signature[8];
        uint8_t Checksum;
        char OemID[6];
        uint8_t Revision;
        uint32_t RsdtAddress;
    } __attribute__((packed));

    struct ExtendedSystemDescriptionPointer : public RootSystemDescriptionPointer
    {
        uint32_t Length;
        uint64_t XsdtAddress;
        uint8_t ChecksumExtended;
        uint8_t Reserved[3];
    } __attribute__((packed));

    struct RootSystemDescriptionTable : public ACPITable { uint32_t Pointers[]; /* (table.length - sizeof(table)) / 4 */ };
    struct ExtendedSystemDescriptionTable : public ACPITable { uint64_t Pointers[]; /* (table.length - sizeof(table)) / 8 */ };

    extern SizedArrayList<uint8_t, 256> g_Processors;
    extern LinkedList<MadtIso *> g_InterruptOverrides;

    extern char g_OemId[6];

    extern acpi_fadt_t *g_FADT;
    extern acpi_hpet_t *g_HPET;
    extern pci_mcfg_t *g_PCIMcfg;

    void Initialize();
    void *FindTable(const char* str, int index);

    void Reset();

    uint32_t GetRemapIRQ(uint32_t irq);
    
} // namespace ACPI

class TimerImplACPI : public Timer
{
    uint8_t r_PMTTimerLength;
    ACPIAddress *r_xPMTTimerBlock;

    uint16_t m_TimerTicks;

public:
    TimerImplACPI();
    ~TimerImplACPI();

    virtual void Tick();
    virtual uint64_t CurrentTime(TimeSpan span = Millisecond);
    virtual void Sleep(long milliseconds);
}