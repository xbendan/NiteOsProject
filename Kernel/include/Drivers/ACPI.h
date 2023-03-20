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

struct MADTEntry
{
    uint8_t Type;
    uint8_t Length;
} __attribute__((packed));

struct MADT : public ACPITable
{
    uint32_t Address;
    uint32_t Flags;
    MADTEntry Entries[];
} __attribute__((packed));

struct MADTLocalApic : public MADTEntry
{
    uint8_t ProcessorId;
    uint8_t ApicId;
    uint32_t Flags;
} __attribute__((packed));

struct MADTIoApic : public MADTEntry
{
    uint8_t ApicId;
    uint8_t __reserved__;
    uint32_t Address;
    uint32_t GSIB;
} __attribute__((packed));

struct MADTInterruptOverride : public MADTEntry
{
    uint8_t busSource;
    uint8_t irqSource;
    uint32_t gSi;
    uint16_t Flags;
} __attribute__((packed));

struct MADTNonmaskableInterrupt : public MADTEntry
{
    uint8_t ProcessorId;
    uint16_t Flags;
    uint8_t LINT;
} __attribute__((packed));

typedef struct ACPIAddress
{
    uint8_t AddressSpace;
    uint8_t BitWidth;
    uint8_t BitOffset;
    uint8_t AccessSize;
    uint64_t Address;
} acpi_gas_t;

struct FADT : public ACPITable
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
};

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
    MCFGAddress BaseAddresses[];
};

struct HPET : public ACPITable
{
    uint8_t HardwareRevisionId;
    uint8_t Info;
    uint16_t PCIVendorId;
    uint8_t AddressSpaceId;
    uint8_t RegisterBitWidth;
    uint8_t RegisterBitOffset;
    uint8_t __reserved__;
    uint64_t Address;
    uint8_t HPETNumber;
    uint16_t MinimumTick;
    uint8_t PageProtection;
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
    extern SizedArrayList<MADTInterruptOverride *, 256> g_InterruptOverrides;

    extern MADT *g_MADT;
    extern FADT *g_FADT;
    extern HPET *g_HPET;
    extern MCFG *g_MCFG;

    void Initialize();

    void Reset();

    uint32_t GetRemapIRQ(uint32_t irq);
    
} // namespace ACPI

class ACPITimer : public Timer
{
private:
    uint8_t r_PMTTimerLength;
    ACPIAddress *r_xPMTTimerBlock;

    uint16_t m_TimerTicks;

public:
    ACPITimer();
    ~ACPITimer();

    virtual void Tick();
    virtual uint64_t CurrentTime(TimeSpan span = Millisecond);
    virtual void Sleep(long milliseconds);
};