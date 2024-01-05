#include <drivers/pci/device.h>
#include <stdcxx/types.h>

#define XHCI_PORT_OFFSET 0x400

#define USB_CMD_RS (1 << 0)    // Run/Stop
#define USB_CMD_HCRST (1 << 1) // Host Controller Reset
#define USB_CMD_INTE (1 << 2)  // Interrupter enable
#define USB_CMD_HSEE (1 << 3)  // Host System Error enable

#define USB_STS_HCH (1 << 0)  // HCHalted - 0 if CMD_RS is 1
#define USB_STS_HSE (1 << 2)  // Host System Error - set to 1 on error
#define USB_STS_EINT (1 << 3) // Event Interrupt
#define USB_STS_PCD (1 << 4)  // Port change detect
#define USB_STS_SSS (1 << 8)  // Save State Status - 1 when CMD_CSS is 1
#define USB_STS_RSS (1 << 9)  // Restore State Status - 1 when CMD_CRS is 1
#define USB_STS_SRE                                                            \
    (1 << 10) // Save/Restore Error - 1 when error during save or restore
              // operation
#define USB_STS_CNR (1 << 11) // Controller Not Ready - 0 = Ready, 1 = Not Ready
#define USB_STS_HCE (1 << 12) // Host Controller Error

#define USB_CFG_MAXSLOTSEN (0xFF) // Max slots enabled
#define USB_CFG_U3E (1 << 8)      // U3 Entry Enable
#define USB_CFG_CIE (1 << 9)      // Configuration Information Enable

#define USB_CCR_RCS (1 << 0) // Ring Cycle State
#define USB_CCR_CS (1 << 1)  // Command Stop
#define USB_CCR_CA (1 << 2)  // Command Abort
#define USB_CCR_CRR (1 << 3) // Command Ring Running

#define USB_CCR_PTR_LO 0xFFFFFFC0‬
#define USB_CCR_PTR 0xFFFFFFFFFFFFFFC0‬ // Command Ring Pointer

#define XHCI_PORTSC_CCS (1 << 0) // Current Connect Status
#define XHCI_PORTSC_PED (1 << 1) // Port Enabled/Disabled
#define XHCI_PORTSC_OCA (1 << 3) // Overcurrent Active
#define XHCI_PORTSC_PR                                                         \
    (1 << 4) // Port Reset - When software writes a ‘1’ to this bit generating a
             // ‘0’ to ‘1’ transition, the bus reset sequence is initiated
#define XHCI_PORTSC_PP (1 << 9)   // Port Power - When 0 the port is powered off
#define XHCI_PORTSC_CSC (1 << 17) // Connect Status Change
#define XHCI_PORTSC_PEC (1 << 18) // Port Enabled/Disabled Change
#define XHCI_PORTSC_PRC (1 << 21) // Port Reset Change
#define XHCI_PORTSC_WPR (1 << 31) // On USB3 ports warm reset

#define XHCI_INT_ERDP_BUSY (1 << 3)

#define XHCI_TRB_SIZE 16
#define XHCI_EVENT_RING_SEGMENT_TABLE_ENTRY_SIZE 16

namespace Kern::Hal::Specs {
    enum class XhciExtendedCapabilities
    {
        LegacySupport                     = 1,
        SupportedProtocol                 = 2,
        ExtendedPowerManagement           = 3,
        IoVirtualization                  = 4,
        MessageSignaledInterrupts         = 5,
        LocalMemory                       = 6,
        DebugCapability                   = 10,
        DoorbellArray                     = 11,
        HostCapabilities                  = 12,
        DeviceCapabilities                = 13,
        USBDebugCapability                = 14,
        ExtendedMessageSignaledInterrupts = 15,
        SecondaryPCIeCapability           = 16,
        MultiHostCapability               = 17,
    };

    struct XhciCapabilitiesRegs
    {
        uint8_t  _capRegLength;
        uint8_t  _reserved;
        uint16_t _hciVersion;
        uint32_t _hcsParams1;
        uint32_t _hcsParams2;
        uint32_t _hcsParams3;
        union
        {
            uint32_t _hccParams1;
            struct
            {
                uint32_t _64bitsCapability : 1;
                uint32_t _bwNegotationCapability : 1;
                uint32_t _contextSize : 1;
                uint32_t _portPowerControl : 1;
                uint32_t _portIndicators : 1;
                uint32_t _lightHCResetCapability : 1;
                uint32_t _latencyToleranceMessagingCapability : 1;
                uint32_t _noSecondarySIDSupport : 1;
                uint32_t _parseAllEventData : 1;
                uint32_t _shortPacketCapability : 1;
                uint32_t _stoppedEdtlaCapability : 1;
                uint32_t _contiguousFrameIDCapability : 1;
                uint32_t _maxPrimaryStreamArraySize : 4;
                uint32_t _xhciExtendedCapabilitiesPointer : 16;
            } __attribute__((packed));
        };
    };

    struct XhciOperationalRegs
    {
        uint32_t _usbCommand;
        uint32_t _usbStatus;
        uint32_t _pageSize;
        uint32_t _reserved;
        uint64_t _deviceNotificationControl;
        union
        {
            uint64_t _commandRingControl;
            struct
            {
                uint64_t _ringCycleState : 1;
                uint64_t _ringCmdStop : 1;
                uint64_t _ringCmdAbort : 1;
                uint64_t _ringCmdRunning : 1;
                uint64_t _reserved : 2;
                uint64_t _ringCmdPtr : 58;
            } __attribute__((packed));
        };
        uint8_t  _reserved2[16];
        uint64_t _deviceContextBaseAddressArrayPointer;
        uint64_t _config;
    };
}