#include <
#include <macros>

namespace AHCI {
    enum FisRegisters
    {
        FisTypeRegH2D = 0x27, // Register FIS - host to device
        FisTypeRegD2H = 0x34, // Register FIS - device to host
        FisTypeDMAAct = 0x39, // DMA activate FIS - device to host
        FisTypeDMASetup = 0x41, // DMA setup FIS - bidirectional
        FisTypeData = 0x46, // Data FIS - bidirectional
        FisTypeBIST = 0x58, // BIST activate FIS - bidirectional
        FisTypePIOSetup = 0x5F, // PIO setup FIS - device to host
        FisTypeDeviceBits = 0xA1 // Set device bits FIS - device to host
    };

    struct FisHostToDevice
    {
        // DWORD 0
        uint8_t FisType /* FIS_TYPE_REG_H2D */;

        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 3;
        uint8_t Mode: 1;

        uint8_t Command;
        uint8_t FeatureLow;

        // DWORD 1
        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        // DWORD 2
        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t FeatureHigh;

        // DWORD 3
        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t ICC; // Isochronous Command Completion
        uint8_t Control;

        // DWORD 4
        uint8_t __reserved__1[4];
    };

    struct FisDeviceToHost
    {
        // DWORD 0
        uint8_t FisType /* FIS_TYPE_REG_D2H */;

        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 2;
        uint8_t Interrupt: 1;
        uint8_t __reserved__1: 1;

        uint8_t Status;
        uint8_t Error;

        // DWORD 1
        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        // DWORD 2
        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t __reserved__2;

        // DWORD 3
        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t __reserved__3[2];

        // DWORD 4
        uint8_t __reserved__4[4];
    };

    struct FisData
    {
        uint8_t FisType /* FIS_TYPE_DATA */;
        
        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 4;

        uint8_t __reserved__1[2];

        uint32_t data[1]; // Payload
    };

    struct FisSetupPIO
    {
        // DWORD 0
        uint8_t FisType /* FIS_TYPE_REG_D2H */;

        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 1;
        uint8_t TransferDirection: 1;
        uint8_t Interrupt: 1;
        uint8_t __reserved__1: 1;

        uint8_t Status;
        uint8_t Error;

        // DWORD 1
        uint8_t LBA0;
        uint8_t LBA1;
        uint8_t LBA2;
        uint8_t Device;

        // DWORD 2
        uint8_t LBA3;
        uint8_t LBA4;
        uint8_t LBA5;
        uint8_t __reserved__2;

        // DWORD 3
        uint8_t CountLow;
        uint8_t CountHigh;
        uint8_t __reserved__3;
        uint8_t NewStatus; // New value of status register

        // DWORD 4
        uint16_t TransferCount;
        uint8_t __reserved__4[2];
    };

    struct FisSetupDMA
    {
        uint8_t FisType /* FIS_TYPE_DMA_SETUP */;
        
        uint8_t PortMultiplier: 4;
        uint8_t __reserved__0: 1;
        uint8_t TransferDirection: 1;
        uint8_t Interrupt: 1;
        uint8_t AutoActivate: 1;

        uint8_t __reserved__0[2];

        
        uint64_t BufferId;  // DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
                            // SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

        uint32_t __reserved__1;

        uint32_t BufferOffset;

        uint32_t TransferCount;

        uint32_t __reserved__2;
    }; 

    class HBADiskController
    {
        
    }
}