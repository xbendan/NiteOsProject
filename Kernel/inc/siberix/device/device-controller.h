#include <siberix/device/device.h>
#include <stdcxx/linked-list.h>

namespace Kern::Device {
    class DeviceManagement
    {
    public:
        Device*                   findDevice(Std::String<Utf8> name);
        Device*                   findDevice(Std::UUID uuid);
        Device*                   findDevice(UInt64 deviceId);
        Std::LinkedList<Device&>& getAllDevices();
        UInt64                    count();

    private:
        Std::LinkedList<Device&> m_devices;
        Boolean                  m_isAutoConnect;
    };
}