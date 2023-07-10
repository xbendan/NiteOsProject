#include <siberix/device/types.hpp>

class ConnectivityManagement {
public:
    Device*              getDevice(const char* str);
    Device*              getDevice(u64 deviceId);
    bool                 install(Device& device);
    bool                 uninstall(Device& device);
    bool                 uninstall(u64 deviceId);
    LinkedList<Device&>& enumerateDevices(DeviceType type);
    u32                  count();

private:
    LinkedList<Device&> m_deviceList[DeviceTypeUnknown + 1];
    u32                 m_connectedDeviceCount;
};