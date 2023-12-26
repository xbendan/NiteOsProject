#include <drivers/pci/info.h>
#include <drivers/pci/spec.h>
#include <siberix/dvc/device.h>
#include <siberix/hwports.h>
#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

namespace PCI {
    using namespace Kern::Hal;

    class PCIDevice
      : protected PCIInfo
      , public Kern::Device
    {
    public:
        PCIDevice(uint8_t bus, uint8_t slot, uint8_t func);
        PCIDevice(uint8_t  bus,
                  uint8_t  slot,
                  uint8_t  func,
                  uint16_t vendorID,
                  uint16_t deviceID);
        PCIDevice(PCIInfo& info);

        // clang-format off

        inline uint8_t  getInterruptLine()              { return read<uint8_t>(Regs::InterruptLine); }
        inline void     setInterruptLine(uint8_t value) { write<uint8_t>(Regs::InterruptLine, value); }
        inline uint8_t  getInterruptPin()               { return read<uint8_t>(Regs::InterruptPin); }
        inline void     setInterruptPin(uint8_t value)  { write<uint8_t>(Regs::InterruptPin, value); }
        inline uint8_t  getCommand()                    { return read<uint8_t>(Regs::Command); } 
        inline void     setCommand(uint8_t value)       { write<uint8_t>(Regs::Command, value); }
        inline uint8_t  getStatus()                     { return read<uint8_t>(Regs::Status); }
        inline uint8_t  getHeaderType()                 { return read<uint8_t>(Regs::HeaderType); }
        inline void     enableBusMastering()            { setCommand(getCommand() | PCI_CMD_BUS_MASTER); }
        inline void     enableInterrupts()              { setCommand(getCommand() & (~PCI_CMD_INTERRUPT_DISABLE)); }
        inline void     enableMemorySpace()             { setCommand(getCommand() | PCI_CMD_MEMORY_SPACE); }
        inline void     enableIO()                      { setCommand(getCommand() | PCI_CMD_IO_SPACE); }

        // clang-format on

    private:
        inline uint32_t address(uint8_t offset)
        {
            return (_bus << 16) | (_slot << 11) | (_func << 8) |
                   (offset & 0xfc) | ((uint32_t)0x80000000);
        }
    };
}