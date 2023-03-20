#pragma once

#include <Arch/x86_64/mmu.h>
#include <Utils/LinkedList.h>
#include <macros>

enum AcpiTableNameDefinition
{
    AcpiMultiDescriptionTable = 0
};

typedef struct ACPITable
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemId[6];
    char oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
} __attribute__((packed)) acpi_header_t;

typedef struct MADTEntry
{
    uint8_t type;
    uint8_t length;
} __attribute__((packed)) madt_entry_t;

typedef struct AcpiMadt : public ACPITable
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
    uint16_t flags;
} __attribute__((packed)) madt_iso_t;

typedef struct MadtNmi : public MADTEntry
{
    uint8_t processorId;
    uint16_t flags;
    uint8_t lInt;
} __attribute__((packed)) madt_nmi_t;

typedef struct ACPIAddress
{
    uint8_t addressSpace;
    uint8_t BitWidth;
    uint8_t bitOffset;
    uint8_t AccessSize;
    uint64_t address;
} acpi_gas_t;

typedef struct Fadt : public ACPITable
{
    uint32_t firmware_control;
    uint32_t dsdt;

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
    uint32_t flags;

    struct ACPIAddress ResetRegistry;

    uint8_t ResetVal;
    uint16_t ARMBootArch;
    uint8_t MinorVersion;

    uint64_t x_firmware_control;
    uint64_t x_dsdt;

    struct ACPIAddress xPM1aEventBlock;
    struct ACPIAddress xPM1bEventBlock;
    struct ACPIAddress xPM1aControlBlock;
    struct ACPIAddress xPM1bControlBlock;
    struct ACPIAddress xPM2ControlBlock;
    struct ACPIAddress xPMTTimerBlock;
    struct ACPIAddress xGPE0Block;
    struct ACPIAddress xGPE1Block;
} acpi_fadt_t;

typedef struct MadtAddressOverride : public MADTEntry
{
    uint16_t reserved;
    uint64_t address;
} madt_address_override_t;

struct MCFGAddress
{
    uint64_t Base;
    uint16_t SegmentGroupNumber;
    uint8_t BusStart;
    uint8_t BusEnd;
    uint32_t __reserved__;
};

typedef struct MCFG : public ACPITable
{
    uint64_t __reserved__;
    MCFGAddress BaseAddresses[];
} pci_mcfg_t;

typedef struct HPET : public ACPITable
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
} acpi_hpet_t;

typedef struct ACPIRootSystemDescriptionPointer
{
    char signature[8];
    uint8_t checksum;
    char oemId[6];
    uint8_t revision;
    uint32_t rsdtPtr;
} __attribute__((packed)) acpi_rsdp_t;

typedef struct AcpiRootSystemDescPtr2
{
    acpi_rsdp_t rsdpPart;
    uint32_t length;
    uint64_t xsdtPtr;
    uint8_t checksumExtended;
    uint8_t reserved[3];
} __attribute__((packed)) acpi_xsdp_t;

typedef struct AcpiSystemDescTable
{
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oemId[6];
    char oemTableId[8];
    uint32_t oemRevision;
    uint32_t creatorId;
    uint32_t creatorRevision;
} acpi_sdt_t;

typedef struct ACPIRootSdt
{
    struct AcpiSystemDescTable table;
    uint32_t pointers[]; // (table.length - sizeof(table)) / 4
} acpi_rsdt_t;

typedef struct ACPIExtendedSdt
{
    struct AcpiSystemDescTable table;
    uint64_t pointers[]; // (table.length - sizeof(table)) / 8
} acpi_xsdt_t;

namespace ACPI
{
    extern uint8_t g_Processors[];
    extern int g_ProcessorCount;
    extern madt_iso_t *g_Isos[256];
    extern uint8_t g_IsoAmount;

    extern acpi_fadt_t *acpiFadt;
    extern acpi_hpet_t *acpiHpet;
    extern pci_mcfg_t *pciMcfg;

    void Initialize();
    void *FindTable(const char* str, int index);

    void SetRsdpTable(acpi_xsdt_t *pointer);
    void Reset();

    uint32_t GetRemapIRQ(uint32_t irq);

    namespace Timer
    {
        extern uint16_t timerTicks;
        void Initialize();
        void Sleep(uint64_t microsecs);
    } // namespace Timer
    
} // namespace ACPI


