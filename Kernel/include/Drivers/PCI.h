#include <macros>
#include <Device/Device.h>

#define PCI_BIST_CAPABLE (1 << 7)
#define PCI_BIST_START (1 << 6)
#define PCI_BIST_COMPLETION_CODE (0xF)

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
#define PCI_CAP_MSI_CONTROL_SET_MME(x) ((x & 0x7) << 4) // Multiple message enable
#define PCI_CAP_MSI_CONTROL_MMC(x) ((x >> 1) & 0x7)     // Multiple Message Capable
#define PCI_CAP_MSI_CONTROL_ENABLE (1 << 0)             // MSI Enable

#define PCI_DEVICE_ID 0x2
#define PCI_VENDOR_ID 0x0
#define PCI_STATUS 0x6
#define PCI_COMMAND 0x4
#define PCI_CLASS_CODE 0xb
#define PCI_SUB_CLASS 0xa
#define PCI_PROG_IF 0x9
#define PCI_REVISION_ID 0x8
#define PCI_BIST 0xf
#define PCI_HEADER_TYPE 0xe
#define PCI_LATENCY_TIMER 0xd
#define PCI_CACHE_LINE_SIZE 0xc
#define PCI_BAR0 0x10
#define PCI_BAR1 0x14
#define PCI_BAR2 0x18
#define PCI_BAR3 0x1c
#define PCI_BAR4 0x20
#define PCI_BAR5 0x24
#define PCI_CARDBUS_CIS_POINTER 0x28
#define PCI_SUBSYSTEM_ID 0x2e
#define PCI_SUBSYSTEM_VENDOR 0x2c
#define PCI_EXPANSION_ROM_BASE_ADDRESS 0x30
#define PCI_CAPABILITIES_POINTER 0x34,
#define PCI_MAX_LATENCY 0x3f
#define PCI_MIN_GRANT 0x3e
#define PCI_INTERRUPT_PIN 0x3d
#define PCI_INTERRUPT_LINE 0x3c

#define PCI_PACKAGE_ADDRESS(bus, slot, func, offset) ((bus << 16 | slot << 11 | func << 8 | offset & 0xFC | 0x80000000))

namespace PCI
{
    struct MSICapability
    {
        union
        {
            struct
            {
                uint32_t capID : 8;       // Should be PCICapMSI
                uint32_t nextCap : 8;     // Next Capability
                uint32_t msiControl : 16; // MSI control register
            } __attribute__((packed));
            uint32_t register0;
        };
        union
        {
            uint32_t addressLow; // Interrupt Message Address Low
            uint32_t register1;
        };
        union
        {
            uint32_t data;        // MSI data
            uint32_t addressHigh; // Interrupt Message Address High (64-bit only)
            uint32_t register2;
        };
        union
        {
            uint32_t data64; // MSI data when 64-bit capable
            uint32_t register3;
        };
    } __attribute__((packed));

    struct PCIInfo
    {
        uint16_t deviceId;
        uint16_t vendorId;
        
        uint8_t bus;
        uint8_t slot;
        uint8_t func;

        uint8_t classCode;
        uint8_t subclass;
        uint8_t progIf;
    };

    struct PCIDevice
    {
        uint16_t deviceId;
        uint16_t vendorId;
        uint8_t bus;
        uint8_t slot;
        uint8_t func;
        uint8_t classCode;
        uint8_t subClass;
        uint8_t progIf;
        uint8_t msiPtr;
        MSICapability msiCap;
        bool msiCapable;
    };

    enum PCIVendors
    {
        VendorAMD = 0x1022,
        VendorIntel = 0x8086
    };

    enum ConfigurationAccessMode
    {
        Legacy,   // PCI
        Enhanced, // PCI Express
    };

    class PCIDeviceProdiver : public DeviceProvider
    {
        
    }

    void MsiCapSetData(MSICapability* msiCap, uint32_t dat);
    uint32_t MsiCapGetData(MSICapability* msiCap);
    void MsiCapSetAddress(MSICapability* msiCap, int cpu);

    void SetDevice(PCIDevice *info, uint8_t bus, uint8_t slot, uint8_t func);
    void SetDeviceFromInfo(const PCIInfo *info);

    uint32_t ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint16_t ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint8_t ConfigReadByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

    void ConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);
    void ConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data);
    void ConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data);

    bool CheckDevice(uint8_t bus, uint8_t device, uint8_t func);
    bool FindDevice(uint16_t deviceID, uint16_t vendorID);
    bool FindGenericDevice(uint16_t classCode, uint16_t subclass);

    void EnumerateDevice(uint16_t deviceID, uint16_t vendorID, void(*func)(const PCIInfo *info));
    void EnumerateGenericDevice(uint8_t classCode, uint8_t subclass, void(*func)(const PCIInfo *info));

    inline uintptr_t GetBaseAddressRegister(PCIDevice *device, uint8_t idx)
    {
        uintptr_t bar = ConfigReadDword(
            device->bus,
            device->slot,
            device->func,
            PCI_BAR0 + (idx * sizeof(uint32_t))
        );
        if(!(bar & 0x1) && bar & 0x4 && idx < 5)
        {
            bar |= ((uintptr_t)(ConfigReadDword(
                device->bus,
                device->slot,
                device->func,
                PCI_BAR0 + ((bar + 1) * sizeof(uint32_t))
            ))) << 32;
        }
        return (bar & 0x1) ? (bar & 0xFFFFFFFFFFFFFFFC) : (bar & 0xFFFFFFFFFFFFFFF0);
    }

    inline bool IsBarIOPort(PCIDevice *device, uint8_t idx) 
    {
        return ConfigReadDword(
            device->bus,
            device->slot,
            device->func,
            PCI_BAR0 + (idx * sizeof(uint32_t))
        ) & 0x1;
    }

    inline uint8_t GetInterruptLine(PCIDevice *device)
    {
        return ConfigReadByte(
            device->bus,
            device->slot,
            device->func,
            PCI_INTERRUPT_LINE
        );
    }

    inline void SetInterruptLine(PCIDevice *device, uint8_t irq)
    {
        ConfigWriteByte(
            device->bus,
            device->slot,
            device->func,
            PCI_INTERRUPT_LINE,
            irq
        );
    }

    inline uint16_t GetCommand(PCIDevice *device)
    {
        return ConfigReadWord(
            device->bus,
            device->slot,
            device->func,
            PCI_COMMAND
        );
    }

    inline void SetCommand(PCIDevice *device, uint16_t val)
    {
        ConfigWriteWord(
            device->bus,
            device->slot,
            device->func,
            PCI_COMMAND,
            val
        );
    }

    inline void EnableBusMastering(PCIDevice *device)
    {
        ConfigWriteWord(
            device->bus,
            device->slot,
            device->func,
            PCI_COMMAND,
            ConfigReadWord(
                device->bus,
                device->slot,
                device->func,
                PCI_COMMAND
            ) | PCI_CMD_BUS_MASTER
        );
    }
} // namespace PCI