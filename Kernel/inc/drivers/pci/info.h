#include <drivers/pci/spec.h>
#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

namespace Kern::Hal::Impls {
    class PCIInfo
    {
    public:
        template <typename T>
            requires(Std::IsSame<T, uint8_t> || Std::IsSame<T, uint16_t> ||
                     Std::IsSame<T, uint32_t>)
        T read(uint8_t offset)
        {
            portRegsWrite<uint32_t>(PCI_CONFIG_ADDRESS, address(offset));

            T data = portRegsRead<uint32_t>(PCI_CONFIG_DATA);
            switch (sizeof(T)) {
                case 1:
                    return data >> ((offset & 3) * 8) & 0xff;
                case 2:
                    return data >> ((offset & 2) * 8) & 0xffff;
                case 4:
                    return data;
            }
            // TODO: throw exception
        }

        template <typename T>
        T read(PCIConfigRegs reg)
        {
            return read<T>(static_cast<uint8_t>(reg));
        }

        template <typename T>
            requires(Std::IsSame<T, uint8_t> || Std::IsSame<T, uint16_t> ||
                     Std::IsSame<T, uint32_t>)
        void write(uint8_t offset, T value)
        {
            portRegsWrite<uint32_t>(PCI_CONFIG_ADDRESS, address(offset));

            T data = portRegsRead<uint32_t>(PCI_CONFIG_DATA);
            switch (sizeof(T)) {
                case 1:
                    data &= ~(0xff << ((offset & 3) * 8));
                    data |= (value & 0xff) << ((offset & 3) * 8);
                    break;
                case 2:
                    data &= ~(0xffff << ((offset & 2) * 8));
                    data |= (value & 0xffff) << ((offset & 2) * 8);
                    break;
                case 4:
                    data = value;
                    break;
            }
            portRegsWrite<uint32_t>(PCI_CONFIG_DATA, data);
        }

        template <typename T>
        void write(PCIConfigRegs reg, T value)
        {
            write<T>(static_cast<uint8_t>(reg), value);
        }

        uint8_t getBus() { return _bus; }
        uint8_t getSlot() { return _slot; }
        uint8_t getFunc() { return _func; }

        uint16_t getVendorID()
        {
            return _vendorID
                     ? _vendorID
                     : (_vendorID = read<uint16_t>(PCIConfigRegs::VendorID));
        }

        uint16_t getDeviceID()
        {
            return _deviceID
                     ? _deviceID
                     : (_deviceID = read<uint16_t>(PCIConfigRegs::DeviceID));
        }

        uint8_t getClass()
        {
            return _class ? _class
                          : (_class = read<uint8_t>(PCIConfigRegs::Class));
        }

        uint8_t getSubclass()
        {
            return _subclass
                     ? _subclass
                     : (_subclass = read<uint8_t>(PCIConfigRegs::Subclass));
        }

        uint8_t getProgIF()
        {
            return _progIF ? _progIF
                           : (_progIF = read<uint8_t>(PCIConfigRegs::ProgIF));
        }

        uint16_t _vendorID, _deviceID;
        uint8_t  _bus, _slot, _func;
        uint8_t  _class, _subclass, _progIF;
    };
}