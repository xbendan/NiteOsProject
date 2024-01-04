#include <drivers/pci/info.h>
#include <drivers/pci/spec.h>
#include <siberix/dvc/device.h>
#include <siberix/hwports.h>
#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

class IPeriCompDevice
  : protected PCIInfo
  , public Kern::IDevice
{
public:
    IPeriCompDevice(uint8_t bus, uint8_t slot, uint8_t func);
    IPeriCompDevice(uint8_t  bus,
                    uint8_t  slot,
                    uint8_t  func,
                    uint16_t vendorID,
                    uint16_t deviceID);
    IPeriCompDevice(PCIInfo& info);

    // clang-format off

        inline uint8_t  getInterruptLine()              { return read<uint8_t>(PCIConfigRegs::InterruptLine); }
        inline void     setInterruptLine(uint8_t value) { write<uint8_t>(PCIConfigRegs::InterruptLine, value); }
        inline uint8_t  getInterruptPin()               { return read<uint8_t>(PCIConfigRegs::InterruptPin); }
        inline void     setInterruptPin(uint8_t value)  { write<uint8_t>(PCIConfigRegs::InterruptPin, value); }
        inline uint8_t  getCommand()                    { return read<uint8_t>(PCIConfigRegs::Command); } 
        inline void     setCommand(uint8_t value)       { write<uint8_t>(PCIConfigRegs::Command, value); }
        inline uint8_t  getStatus()                     { return read<uint8_t>(PCIConfigRegs::Status); }
        inline uint8_t  getHeaderType()                 { return read<uint8_t>(PCIConfigRegs::HeaderType); }
        inline void     enableBusMastering()            { setCommand(getCommand() | PCI_CMD_BUS_MASTER); }
        inline void     enableInterrupts()              { setCommand(getCommand() & (~PCI_CMD_INTERRUPT_DISABLE)); }
        inline void     enableMemorySpace()             { setCommand(getCommand() | PCI_CMD_MEMORY_SPACE); }
        inline void     enableIO()                      { setCommand(getCommand() | PCI_CMD_IO_SPACE); }

    // clang-format on

private:
    inline uint32_t address(uint8_t offset)
    {
        return (_bus << 16) | (_slot << 11) | (_func << 8) | (offset & 0xfc) |
               ((uint32_t)0x80000000);
    }
};