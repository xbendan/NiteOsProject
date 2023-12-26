#include <siberix/dvc/device.h>
#include <stdcxx/linked-list.h>

namespace Kern {
    class DeviceEnumerator : public Device
    {
    public:
        DeviceEnumerator(Std::String<Utf8> name)
          : Device(name, DeviceType::SoftwareDevice)
        {
        }

        virtual Std::Array<Device&>& scan() = 0;
    };

    class DeviceHost
    {
    public:
        DeviceHost();

        Device*                   findDevice(Std::String<Utf8> name);
        Device*                   findDevice(Std::UUID uuid);
        Device*                   findDevice(uint64_t deviceId);
        Std::LinkedList<Device&>& getAllDevices();
        uint64_t                  count();

    private:
        Std::LinkedList<Device*> m_devices;
        bool                     m_isAutoConnect;
    };
}