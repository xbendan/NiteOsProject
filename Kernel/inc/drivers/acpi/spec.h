#pragma once

#include <stdcxx/types.h>

namespace Kern::Hal::Specs {
    struct AcpiRsdp
    {
        char     _signature[8];
        uint8_t  _checksum;
        char     _oemId[6];
        uint8_t  _revision;
        uint32_t _rsdtAddress;
    } __attribute__((packed));

    struct AcpiXsdp : public AcpiRsdp
    {
        uint32_t _length;
        uint64_t _xsdtAddress;
        uint8_t  _checksumEx;
        uint8_t  _reserved[3];
    };

    struct AcpiTable
    {
        char     _signature[4];
        uint32_t _length;
        uint8_t  _revision;
        uint8_t  _checksum;
        char     _oemId[6];
        char     _oemTableId[8];
        uint32_t _oemRevision;
        uint32_t _creatorId;
        uint32_t _creatorRevision;
    } __attribute__((packed));

    struct AcpiRsdt : public AcpiTable
    {
        uint32_t _pointers[];
    } __attribute__((packed));

    struct AcpiXsdt : public AcpiTable
    {
        uint64_t _pointers[];
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
        uint8_t  _addressSpace;
        uint8_t  _bitWidth;
        uint8_t  _bitOffset;
        uint8_t  _accessSize;
        uint64_t _address;
    };

    struct MadtEntry
    {
        uint8_t _type;
        uint8_t _length;
    } __attribute__((packed));

    struct Madt /* Multiple APIC Description Table */ : public AcpiTable
    {
        uint32_t  _address;
        uint32_t  _flags;
        MadtEntry _entries[];
    } __attribute__((packed));

    struct MadtLocalApic : public MadtEntry
    {
        uint8_t  _processorId;
        uint8_t  _apicId;
        uint32_t _flags;
    } __attribute__((packed));

    struct MadtIoApic : public MadtEntry
    {
        uint8_t  _apicId;
        uint8_t  __reserved__;
        uint32_t _address;
        uint32_t _gSiB;
    } __attribute__((packed));

    struct MadtIso : public MadtEntry
    {
        uint8_t  _busSource;
        uint8_t  _irqSource;
        uint32_t _gsi;
        uint16_t _flags;
    } __attribute__((packed));

    struct MadtNmi : public MadtEntry
    {
        uint8_t  _processorId;
        uint16_t _flags;
        uint8_t  _lInt;
    } __attribute__((packed));

    struct MadtLocalx2Apic : public MadtEntry
    {
        uint16_t __reserved__;
        uint32_t _x2apicId;
        uint32_t _flags;
        uint32_t _uid;
    } __attribute__((packed));

    struct MadtNmix2Apic : public MadtEntry
    {
        uint16_t _flags;
        uint32_t _uid;
        uint8_t  _lInt;
        uint8_t  __reserved__[3];
    } __attribute__((packed));

    struct Hpet : public AcpiTable
    {
        uint8_t     _hwrevId;
        uint8_t     _info;
        uint16_t    _pciVendorId;
        AcpiAddress _address;
        uint8_t     _hpetNumber;
        uint16_t    _minTick;
        uint8_t     _pageProtection;
    };

    struct McfgAddress
    {
        uint64_t _base;
        uint16_t _sgn;
        uint8_t  _busStart;
        uint8_t  _busEnd;
        uint32_t __reserved__;
    };

    struct PciMcfg : public AcpiTable
    {
        uint64_t    __reserved__;
        McfgAddress _baseAddresses[];
    };

    struct AcpiFadt /* Fixed ACPI Description Table */ : public AcpiTable
    {
        uint32_t _fwctrl;
        uint32_t _dsdt;

        uint8_t __reserved__0;

        uint8_t  _ppmp;
        uint16_t _sciInt;
        uint32_t _smiCmd;
        uint8_t  _enable;
        uint8_t  _disable;
        uint8_t  _s4biosReq;
        uint8_t  _pStateCtrl;
        uint32_t _pm1aEventBlock;
        uint32_t _pm1bEventBlock;
        uint32_t _pm1aCtrlBlock;
        uint32_t _pm1bCtrlBlock;
        uint32_t _pmtTimerBlock;
        uint32_t _gpe0Block;
        uint32_t _gpe1Block;
        uint8_t  _pm1EventLength;
        uint8_t  _pm1CtrlLength;
        uint8_t  _pm2CtrlLength;
        uint8_t  _pmtTimerLength;
        uint8_t  _gpe0Length;
        uint8_t  _gpe1Length;
        uint8_t  _gpe1Base;
        uint8_t  _cStateCtrl;
        uint16_t _worstC2Latency;
        uint16_t _worstC3Latency;
        uint16_t _flushSize;
        uint16_t _flushStride;
        uint8_t  _dutyOffset;
        uint8_t  _dutyWidth;
        uint8_t  _dayAlarm;
        uint8_t  _monthAlarm;
        uint8_t  _century;

        uint16_t _bootFlagIAPC;

        uint8_t __reserved__1;

        uint32_t _flags;

        AcpiAddress _resetRegistry;
        uint8_t     _resetValue;
        uint16_t    _bootFlagARM;
        uint8_t     _minor;

        uint64_t    x_fwctrl;
        uint64_t    x_dsdt;
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
        uint32_t _warningEnergyLevel;
        uint32_t _lowEnergyLevel;
        uint32_t _criticalEnergyLevel;
    };

    struct AcpiEcdt /* Embedded Controller Boot Resources Table */
      : public AcpiTable
    {
        AcpiAddress _ecCtrl;
        AcpiAddress _ecData;
        uint32_t    _uid;
        uint8_t     _gpeBit;
        uint8_t     _ecId[];
    };

    struct AcpiDsdt /* Differentiated System Description Table */
      : public AcpiTable
    {
        uint8_t _aml[];
    };

    struct AcpiSsdt /* Secondary System Description Table */ : public AcpiTable
    {
        uint8_t _aml[];
    };
}