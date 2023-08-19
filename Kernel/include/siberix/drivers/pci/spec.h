#include <common/typedefs.h>

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
#define PCI_CAP_MSI_CONTROL_64 (1 << 7)              // 64-bit address capable
#define PCI_CAP_MSI_CONTROL_VECTOR_MASKING (1 << 8)  // Enable Vector Masking
#define PCI_CAP_MSI_CONTROL_MME_MASK (0x7U << 4)
#define PCI_CAP_MSI_CONTROL_SET_MME(x) ((x & 0x7) << 4)  // Multiple message enable
#define PCI_CAP_MSI_CONTROL_MMC(x) ((x >> 1) & 0x7)      // Multiple Message Capable
#define PCI_CAP_MSI_CONTROL_ENABLE (1 << 0)              // MSI Enable

#define PCI_PACKAGE_ADDRESS(bus, slot, func, offset) \
    (((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000))

enum PCIConfigRegisters {
    PCIDeviceID                = 0x2,
    PCIVendorID                = 0x0,
    PCIStatus                  = 0x6,
    PCICommand                 = 0x4,
    PCIClassCode               = 0xB,
    PCISubclass                = 0xA,
    PCIProgIF                  = 0x9,
    PCIRevisionID              = 0x8,
    PCIBIST                    = 0xF,
    PCIHeaderType              = 0xE,
    PCILatencyTimer            = 0xD,
    PCICacheLineSize           = 0xC,
    PCIBAR0                    = 0x10,
    PCIBAR1                    = 0x14,
    PCIBAR2                    = 0x18,
    PCIBAR3                    = 0x1C,
    PCIBAR4                    = 0x20,
    PCIBAR5                    = 0x24,
    PCICardbusCISPointer       = 0x28,
    PCISubsystemID             = 0x2E,
    PCISubsystemVendorID       = 0x2C,
    PCIExpansionROMBaseAddress = 0x30,
    PCICapabilitiesPointer     = 0x34,
    PCIMaxLatency              = 0x3F,
    PCIMinGrant                = 0x3E,
    PCIInterruptPIN            = 0x3D,
    PCIInterruptLine           = 0x3C,
};

struct MSICompability {
    union {
        struct {
            u32 capId : 8;        // Should be PCICapMSI
            u32 nextCap : 8;      // Next Capability
            u32 msiControl : 16;  // MSI control register
        } __attribute__((packed));
        u32 reg0;
    };
    union {
        u32 addressLow;  // Interrupt Message Address Low
        u32 reg1;
    };
    union {
        u32 data;         // MSI data
        u32 addressHigh;  // Interrupt Message Address High (64-bit only)
        u32 reg2;
    };
    union {
        u32 data64;
        u32 reg3;
    };
} __attribute__((packed));

enum class PCIVendors {
    AMD   = 0x1022,
    Intel = 0x8086
};

enum class PCIConfigAccessMode {
    Legacy,
    Enhanced
};

class PCIInfo {
public:
    PCIInfo(u8 bus, u8 slot, u8 func, u8 classCode, u8 subClass)
        : m_bus(bus),
          m_slot(slot),
          m_func(func),
          m_classCode(classCode),
          m_subClass(subClass) {}
    PCIInfo(u8 bus, u8 slot, u8 func);
    ~PCIInfo();

    void initIO(PCIConfigRegisters reg);

    inline u8 bus() { return m_bus; }
    inline u8 slot() { return m_slot; }
    inline u8 func() { return m_func; }

    inline u16 getDeviceID() { return m_deviceId; }
    inline u16 getVendorID() { return m_vendorId; }

    u8  readByte(PCIConfigRegisters reg);
    u16 readWord(PCIConfigRegisters reg);
    u32 readDWord(PCIConfigRegisters reg);

    void writeByte(PCIConfigRegisters reg, u8 data);
    void writeWord(PCIConfigRegisters reg, u16 data);
    void writeDWord(PCIConfigRegisters reg, u32 data);

private:
    u16 m_deviceId;
    u16 m_vendorId;

    u8 m_bus;
    u8 m_slot;
    u8 m_func;

    u8 m_classCode;
    u8 m_subClass;
    u8 m_progIf;
};