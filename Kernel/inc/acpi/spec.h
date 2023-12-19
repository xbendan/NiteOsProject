#pragma once

#include <stdcxx/types.h>

struct AcpiRsdp
{
    char   _signature[8];
    UInt8  _checksum;
    char   _oemId[6];
    UInt8  _revision;
    UInt32 _rsdtAddress;
} __attribute__((packed));

struct AcpiXsdp : public AcpiRsdp
{
    UInt32 _length;
    UInt64 _xsdtAddress;
    UInt8  _checksumEx;
    UInt8  _reserved[3];
};

struct AcpiTable
{
    char   _signature[4];
    UInt32 _length;
    UInt8  _revision;
    UInt8  _checksum;
    char   _oemId[6];
    char   _oemTableId[8];
    UInt32 _oemRevision;
    UInt32 _creatorId;
    UInt32 _creatorRevision;
} __attribute__((packed));

struct AcpiRsdt : public AcpiTable
{
    UInt32 _pointers[];
} __attribute__((packed));

struct AcpiXsdt : public AcpiTable
{
    UInt64 _pointers[];
} __attribute__((packed));

struct AcpiAddress
{
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
    UInt8  _addressSpace;
    UInt8  _bitWidth;
    UInt8  _bitOffset;
    UInt8  _accessSize;
    UInt64 _address;
};

struct MadtEntry
{
    UInt8 _type;
    UInt8 _length;
} __attribute__((packed));

struct Madt /* Multiple APIC Description Table */ : public AcpiTable
{
    UInt32    _address;
    UInt32    _flags;
    MadtEntry _entries[];
} __attribute__((packed));

struct MadtLocalApic : public MadtEntry
{
    UInt8  _processorId;
    UInt8  _apicId;
    UInt32 _flags;
} __attribute__((packed));

struct MadtIoApic : public MadtEntry
{
    UInt8  _apicId;
    UInt8  __reserved__;
    UInt32 _address;
    UInt32 _gSiB;
} __attribute__((packed));

struct MadtIso : public MadtEntry
{
    UInt8  _busSource;
    UInt8  _irqSource;
    UInt32 _gsi;
    UInt16 _flags;
} __attribute__((packed));

struct MadtNmi : public MadtEntry
{
    UInt8  _processorId;
    UInt16 _flags;
    UInt8  _lInt;
} __attribute__((packed));

struct MadtLocalx2Apic : public MadtEntry
{
    UInt16 __reserved__;
    UInt32 _x2apicId;
    UInt32 _flags;
    UInt32 _uid;
} __attribute__((packed));

struct MadtNmix2Apic : public MadtEntry
{
    UInt16 _flags;
    UInt32 _uid;
    UInt8  _lInt;
    UInt8  __reserved__[3];
} __attribute__((packed));

struct Hpet : public AcpiTable
{
    UInt8       _hwrevId;
    UInt8       _info;
    UInt16      _pciVendorId;
    AcpiAddress _address;
    UInt8       _hpetNumber;
    UInt16      _minTick;
    UInt8       _pageProtection;
};

struct McfgAddress
{
    UInt64 _base;
    UInt16 _sgn;
    UInt8  _busStart;
    UInt8  _busEnd;
    UInt32 __reserved__;
};

struct PciMcfg : public AcpiTable
{
    UInt64      __reserved__;
    McfgAddress _baseAddresses[];
};

struct AcpiFadt /* Fixed ACPI Description Table */ : public AcpiTable
{
    UInt32 _fwctrl;
    UInt32 _dsdt;

    UInt8 __reserved__0;

    UInt8  _ppmp;
    UInt16 _sciInt;
    UInt32 _smiCmd;
    UInt8  _enable;
    UInt8  _disable;
    UInt8  _s4biosReq;
    UInt8  _pStateCtrl;
    UInt32 _pm1aEventBlock;
    UInt32 _pm1bEventBlock;
    UInt32 _pm1aCtrlBlock;
    UInt32 _pm1bCtrlBlock;
    UInt32 _pmtTimerBlock;
    UInt32 _gpe0Block;
    UInt32 _gpe1Block;
    UInt8  _pm1EventLength;
    UInt8  _pm1CtrlLength;
    UInt8  _pm2CtrlLength;
    UInt8  _pmtTimerLength;
    UInt8  _gpe0Length;
    UInt8  _gpe1Length;
    UInt8  _gpe1Base;
    UInt8  _cStateCtrl;
    UInt16 _worstC2Latency;
    UInt16 _worstC3Latency;
    UInt16 _flushSize;
    UInt16 _flushStride;
    UInt8  _dutyOffset;
    UInt8  _dutyWidth;
    UInt8  _dayAlarm;
    UInt8  _monthAlarm;
    UInt8  _century;

    UInt16 _bootFlagIAPC;

    UInt8 __reserved__1;

    UInt32 _flags;

    AcpiAddress _resetRegistry;
    UInt8       _resetValue;
    UInt16      _bootFlagARM;
    UInt8       _minor;

    UInt64      x_fwctrl;
    UInt64      x_dsdt;
    AcpiAddress x_pm1aEventBlock;
    AcpiAddress x_pm1bEventBlock;
    AcpiAddress x_pm1aCtrlBlock;
    AcpiAddress x_pm1bCtrlBlock;
    AcpiAddress x_pm2CtrlBlock;
    AcpiAddress x_pmtTimerBlock;
    AcpiAddress x_gpe0Block;
    AcpiAddress x_gpe1Block;
};

struct AcpiFacs /* Firmware ACPI Control Structure */
{};

struct AcpiSbst /* Smart Battery Description Table */ : public AcpiTable
{
    UInt32 _warningEnergyLevel;
    UInt32 _lowEnergyLevel;
    UInt32 _criticalEnergyLevel;
};

struct AcpiEcdt /* Embedded Controller Boot Resources Table */
  : public AcpiTable
{
    AcpiAddress _ecCtrl;
    AcpiAddress _ecData;
    UInt32      _uid;
    UInt8       _gpeBit;
    UInt8       _ecId[];
};

struct AcpiDsdt /* Differentiated System Description Table */
  : public AcpiTable
{
    UInt8 _aml[];
};

struct AcpiSsdt /* Secondary System Description Table */ : public AcpiTable
{
    UInt8 _aml[];
};