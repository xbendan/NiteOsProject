#pragma once

#include <Device/Device.h>
#include <Drivers/Generic/ACPI.h>
#include <Utils/ArrayList.h>

#include <macros>

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

#define PCI_PACKAGE_ADDRESS(bus, slot, func, offset) ((bus << 16 | slot << 11 | func << 8 | offset & 0xFC | 0x80000000))

namespace PCI
{
    enum PCIConfigRegisters{
        PCIDeviceID = 0x2,
        PCIVendorID = 0x0,
        PCIStatus = 0x6,
        PCICommand = 0x4,
        PCIClassCode = 0xB,
        PCISubclass = 0xA,
        PCIProgIF = 0x9,
        PCIRevisionID = 0x8,
        PCIBIST = 0xF,
        PCIHeaderType = 0xE,
        PCILatencyTimer = 0xD,
        PCICacheLineSize = 0xC,
        PCIBAR0 = 0x10,
        PCIBAR1 = 0x14,
        PCIBAR2 = 0x18,
        PCIBAR3 = 0x1C,
        PCIBAR4 = 0x20,
        PCIBAR5 = 0x24,
        PCICardbusCISPointer = 0x28,
        PCISubsystemID = 0x2E,
        PCISubsystemVendorID = 0x2C,
        PCIExpansionROMBaseAddress = 0x30,
        PCICapabilitiesPointer = 0x34,
        PCIMaxLatency = 0x3F,
        PCIMinGrant = 0x3E,
        PCIInterruptPIN = 0x3D,
        PCIInterruptLine = 0x3C,
    };

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

    uint32_t ConfigReadDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint16_t ConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
    uint8_t ConfigReadByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);

    void ConfigWriteDword(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint32_t data);
    void ConfigWriteWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint16_t data);
    void ConfigWriteByte(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset, uint8_t data);

    struct PCIInfo
    {
        uint16_t m_DeviceID;
        uint16_t m_VendorID;

        uint8_t m_Bus;
        uint8_t m_Slot;
        uint8_t m_Func;

        uint8_t m_ClassCode;
        uint8_t m_SubClass;
        uint8_t m_ProgIf;

        PCIInfo(uint8_t bus, uint8_t slot, uint8_t func, uint8_t classCode, uint8_t subclass)
          : m_Bus(bus),
            m_Slot(slot),
            m_Func(func),
            m_DeviceID(ConfigReadWord(bus, slot, func, PCIDeviceID)),
            m_VendorID(ConfigReadWord(bus, slot, func, PCIVendorID)),
            m_ClassCode(classCode),
            m_SubClass(subclass),
            m_ProgIf(ConfigReadByte(bus, slot, func, PCIProgIF)) {}

        PCIInfo(uint8_t bus, uint8_t slot, uint8_t func)
          : PCIInfo(bus, slot, func, ConfigReadByte(bus, slot, func, PCIClassCode), ConfigReadByte(bus, slot, func, PCISubclass)) {}
        
    };

    class PCIDevice : public Device, public PCIInfo
    {
    private:
        uint8_t m_MsiPtr;
        MSICapability m_MsiCapability;
        bool m_IsMsiCapable;

    public:
        PCIDevice(uint8_t bus, uint8_t slot, uint8_t func, uint8_t classCode, uint8_t subclass);
        PCIDevice(PCIInfo& deviceInfo)
          : PCIDevice(deviceInfo.m_Bus, deviceInfo.m_Slot, deviceInfo.m_Func, deviceInfo.m_ClassCode, deviceInfo.m_SubClass) {}
        PCIDevice(uint8_t bus, uint8_t slot, uint8_t func)
          : PCIDevice(bus, slot, func, ConfigReadByte(bus, slot, func, PCIClassCode), ConfigReadByte(bus, slot, func, PCISubclass)) {}
        ~PCIDevice();

        virtual void Enable();
        virtual void Disable();

        inline uintptr_t GetBaseAddressRegister(uint8_t idx) {
            if (idx < 0 || idx > 5) {
                return 0x0;
            }

            uintptr_t bar = ConfigReadDword(m_Bus, m_Slot, m_Func, PCIBAR0 + (idx * sizeof(uint32_t)));
            if(!(bar & 0x1) /* Not IO */ && bar & 0x4 /* 64-bit */ && idx < 5){
                bar |= static_cast<uintptr_t>(PCI::ConfigReadDword(m_Bus, m_Slot, m_Func, PCIBAR0 + ((bar + 1) * sizeof(uint32_t)))) << 32;
            }

            return (bar & 0x1) ? (bar & 0xFFFFFFFFFFFFFFFC) : (bar & 0xFFFFFFFFFFFFFFF0);
        }
        inline bool BarIsIOPort(uint8_t idx){ return ConfigReadDword(m_Bus, m_Slot, m_Func, PCIBAR0 + (idx * sizeof(uint32_t))) & 0x1; }

	    inline uint8_t GetInterruptLine(){ return ConfigReadByte(m_Bus, m_Slot, m_Func, PCIInterruptLine); }
	    inline void SetInterruptLine(uint8_t irq){ ConfigWriteByte(m_Bus, m_Slot, m_Func, PCIInterruptLine, irq); }

        inline uint16_t GetCommand() { return ConfigReadWord(m_Bus, m_Slot, m_Func, PCICommand); }
	    inline void SetCommand(uint16_t val) { ConfigWriteWord(m_Bus, m_Slot, m_Func, PCICommand, val); }

        inline void EnableBusMastering() { ConfigWriteWord(m_Bus, m_Slot, m_Func, PCICommand, ConfigReadWord(m_Bus, m_Slot, m_Func, PCICommand) | PCI_CMD_BUS_MASTER); }
        inline void EnableInterrupts() { ConfigWriteWord(m_Bus, m_Slot, m_Func, PCICommand, ConfigReadWord(m_Bus, m_Slot, m_Func, PCICommand) & (~PCI_CMD_INTERRUPT_DISABLE)); }
        inline void EnableMemorySpace() { ConfigWriteWord(m_Bus, m_Slot, m_Func, PCICommand, ConfigReadWord(m_Bus, m_Slot, m_Func, PCICommand) | PCI_CMD_MEMORY_SPACE); }
        inline void EnableIOSpace() { ConfigWriteWord(m_Bus, m_Slot, m_Func, PCICommand, ConfigReadWord(m_Bus, m_Slot, m_Func, PCICommand) | PCI_CMD_IO_SPACE); }

        inline uint8_t HeaderType() { return ConfigReadByte(m_Bus, m_Slot, m_Func, PCIHeaderType); }
        inline uint8_t GetProgIF() { return ConfigReadByte(m_Bus, m_Slot, m_Func, PCIProgIF); }
        inline uint16_t Status() { return ConfigReadWord(m_Bus, m_Slot, m_Func, PCIStatus); }

        inline bool HasCapability(uint16_t capability) { return false; }

        inline uint8_t Bus() { return m_Bus; }
        inline uint8_t Slot() { return m_Slot; }
        inline uint8_t Func() { return m_Func; }

        inline uint16_t DeviceID() { return m_DeviceID; }
        inline uint16_t VendorID() { return m_VendorID; }
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

    class PCIDeviceProvider : public DeviceProvider
    {
        Spinlock lock;
        MCFG *m_McfgTable;
        ConfigurationAccessMode m_AccessMode;

    public:
        // Remember change to ArrayList
        LinkedList<PCIInfo> m_DeviceList;
        LinkedList<MCFGAddress *> m_EnhancedBaseAddressList;

        PCIDeviceProvider();
        ~PCIDeviceProvider();

        Device *FindName(const char *str);
        PCIInfo ConnectDevice(uint8_t bus, uint8_t slot, uint8_t func);

        void SetDevice(PCIDevice *info, uint8_t bus, uint8_t slot, uint8_t func);
        void SetDevice(const PCIInfo *info);

        bool CheckDevice(uint8_t bus, uint8_t device, uint8_t func);
        PCIInfo *FindDevice(uint16_t deviceID, uint16_t vendorID);
        PCIInfo *FindGenericDevice(uint16_t classCode, uint16_t subclass);

        void EnumerateDevice(uint16_t deviceID, uint16_t vendorID, void (*consumer)(PCIInfo& deviceInfo));
        void EnumerateGenericDevice(uint8_t classCode, uint8_t subclass, void (*consumer)(PCIInfo& deviceInfo));
    };

    struct PCIClass
    {
        const char *desc;
        void (*builder)(uint8_t bus, uint8_t slot, uint8_t func);
    };

    void MsiCapSetData(MSICapability* msiCap, uint32_t dat);
    uint32_t MsiCapGetData(MSICapability* msiCap);
    void MsiCapSetAddress(MSICapability* msiCap, int cpu);
} // namespace PCI