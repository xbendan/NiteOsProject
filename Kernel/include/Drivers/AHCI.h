#include <Device/Device.h>
#include <Fs/Disk.h>
#include <Utils/LinkedList.h>
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

        uint8_t __reserved__1[2];

        
        uint64_t BufferId;  // DMA Buffer Identifier. Used to Identify DMA buffer in host memory.
                            // SATA Spec says host specific and not in Spec. Trying AHCI spec might work.

        uint32_t __reserved__2;

        uint32_t BufferOffset;

        uint32_t TransferCount;

        uint32_t __reserved__3;
    };

    typedef volatile struct HBAPortRegisters
    {
        uint32_t CLB, CLBU, FB, FBU, IS, IE, CMD;
        uint32_t __reserved__0;
        uint32_t TFD, SIG, SSTS, SCTL, SERR, SACT, CI, SNTF, FBS;
        uint32_t __reserved__1[11];
        uint32_t Vendor[4];
    } hba_port_t;

    typedef volatile struct HBAMemoryRegisters
    {
        uint32_t HostCapability;
        uint32_t GlobalHostControl;
        uint32_t InterruptStatus;
        uint32_t PortImplemented;
        uint32_t Version;
        uint32_t CCCControl;
        uint32_t CCCPorts;
        uint32_t EMLocation;
        uint32_t EMControl;
        uint32_t HostCapabilityExt;
        uint32_t BOHC;
        
        uint8_t __reserved__[0xA0 - 0x2C];

        uint8_t Vendor[0x100 - 0xA0];

        HBAPortRegisters ports;
    } hba_mem_t;

    typedef volatile struct HBAFrameInformation
    {
        FisSetupDMA SetupDMA;
        uint8_t __padding__0[4];

        FisSetupPIO SetupPIO;
        uint8_t __padding__1[12];

        FisDeviceToHost D2H;
        uint8_t __padding__2[4];

        uint8_t SetBits[8];

        uint8_t ufis[64];
        
        uint8_t __reserved__[0x100 - 0xA0];
    } hba_fis_t;

    class SerialATADiskDevice : public DiskDevice
    {

        SerialATADiskDevice();
        ~SerialATADiskDevice();
    };

    class SATAController
    {
    private:
        LinkedList<SerialATADiskDevice> m_DiskList;
    public:
        SATAController(/* args */);
        ~SATAController();
    };
    
    // SATAController::SATAController(/* args */)
    // {
    // }
    
    // SATAController::~SATAController()
    // {
    // }

    void Initialize();
}