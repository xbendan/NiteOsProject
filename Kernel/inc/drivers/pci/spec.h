#pragma once

#include <stdcxx/types.h>

#define PCI_BIST_CAPABLE (1 << 7)
#define PCI_BIST_START (1 << 6)
#define PCI_BIST_COMPLETION_CODE (0xF)

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

#define PCI_CMD_INTERRUPT_DISABLE (1 << 10)
#define PCI_CMD_FAST_BTB_ENABLE (1 << 9)
#define PCI_CMD_SERR_ENABLE (1 << 8)
#define PCI_CMD_PARITY_ERROR_RESPONSE (1 << 6)
#define PCI_CMD_VGA_PALETTE_SNOOP (1 << 5)
#define PCI_CMD_MEMORY_WRITE_INVALIDATE_ENABLE (1 << 4)
#define PCI_CMD_SPECIAL_CYCLES (1 << 3)
#define PCI_CMD_BUS_MASTER (1 << 2)
#define PCI_CMD_MEMORY_SPACE (1 << 1)
#define PCI_CMD_IO_SPACE (1 << 0)

#define PCI_STATUS_CAPABILITIES (1 << 4)

#define PCI_CLASS_UNCLASSIFIED 0x0
#define PCI_CLASS_STORAGE 0x1
#define PCI_CLASS_NETWORK 0x2
#define PCI_CLASS_DISPLAY 0x3
#define PCI_CLASS_MULTIMEDIA 0x4
#define PCI_CLASS_MEMORY 0x5
#define PCI_CLASS_BRIDGE 0x6
#define PCI_CLASS_COMMUNICATION 0x7
#define PCI_CLASS_PERIPHERAL 0x8
#define PCI_CLASS_INPUT_DEVICE 0x9
#define PCI_CLASS_DOCKING_STATION 0xA
#define PCI_CLASS_PROCESSOR 0xB
#define PCI_CLASS_SERIAL_BUS 0xC
#define PCI_CLASS_WIRELESS_CONTROLLER 0xD
#define PCI_CLASS_INTELLIGENT_CONTROLLER 0xE
#define PCI_CLASS_SATELLITE_COMMUNICATION 0xF
#define PCI_CLASS_ENCRYPTON 0x10
#define PCI_CLASS_SIGNAL_PROCESSING 0x11

#define PCI_CLASS_COPROCESSOR 0x40

#define PCI_SUBCLASS_IDE 0x1
#define PCI_SUBCLASS_FLOPPY 0x2
#define PCI_SUBCLASS_ATA 0x5
#define PCI_SUBCLASS_SATA 0x6
#define PCI_SUBCLASS_NVM 0x8

#define PCI_SUBCLASS_ETHERNET 0x0

#define PCI_SUBCLASS_VGA_COMPATIBLE 0x0
#define PCI_SUBCLASS_XGA 0x1

#define PCI_SUBCLASS_AC97 0x1

#define PCI_SUBCLASS_USB 0x3

#define PCI_PROGIF_UHCI 0x20
#define PCI_PROGIF_OHCI 0x10
#define PCI_PROGIF_EHCI 0x20
#define PCI_PROGIF_XHCI 0x30

#define PCI_IO_PORT_CONFIG_ADDRESS 0xCF8
#define PCI_IO_PORT_CONFIG_DATA 0xCFC

#define PCI_CAP_MSI_ADDRESS_BASE 0xFEE00000
#define PCI_CAP_MSI_CONTROL_64 (1 << 7)             // 64-bit address capable
#define PCI_CAP_MSI_CONTROL_VECTOR_MASKING (1 << 8) // Enable Vector Masking
#define PCI_CAP_MSI_CONTROL_MME_MASK (0x7U << 4)
#define PCI_CAP_MSI_CONTROL_SET_MME(x)                                         \
    ((x & 0x7) << 4)                                // Multiple message enable
#define PCI_CAP_MSI_CONTROL_MMC(x) ((x >> 1) & 0x7) // Multiple Message Capable
#define PCI_CAP_MSI_CONTROL_ENABLE (1 << 0)         // MSI Enable

#define PCI_PACKAGE_ADDRESS(bus, slot, func, offset)                           \
    ((bus << 16 | slot << 11 | func << 8 | offset & 0xFC | 0x80000000))

namespace Kern::Hal::Specs {
    enum class PCIConfigRegs : uint8_t
    {
        VendorID                = 0x00,
        DeviceID                = 0x02,
        Command                 = 0x04,
        Status                  = 0x06,
        RevisionID              = 0x08,
        ProgIF                  = 0x09,
        Subclass                = 0x0A,
        Class                   = 0x0B,
        CacheLineSize           = 0x0C,
        LatencyTimer            = 0x0D,
        HeaderType              = 0x0E,
        BIST                    = 0x0F,
        BAR0                    = 0x10,
        BAR1                    = 0x14,
        BAR2                    = 0x18,
        BAR3                    = 0x1C,
        BAR4                    = 0x20,
        BAR5                    = 0x24,
        CardbusCISPointer       = 0x28,
        SubsystemVendorID       = 0x2C,
        SubsystemID             = 0x2E,
        ExpansionROMBaseAddress = 0x30,
        CapabilitiesPointer     = 0x34,
        InterruptLine           = 0x3C,
        InterruptPin            = 0x3D,
        MinGrant                = 0x3E,
        MaxLatency              = 0x3F
    };

    enum PCIConfigAccessMode
    {
        Legacy,
        Enhanced
    };
}