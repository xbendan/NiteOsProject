#include <siberix/device/types.hpp>

class ConnectivityProvider {};

class DeviceConnectivity {
public:
    Device*              getDevice(const char* str);
    Device*              getDevice(u64 deviceId);
    bool                 install(Device& device);
    bool                 uninstall(Device& device);
    bool                 uninstall(u64 deviceId);
    LinkedList<Device&>& enumerateDevices(DeviceType type);
    u32                  count();
    void                 setAutoConnect(bool autoConnect);

private:
    LinkedList<ConnectivityProvider&> m_providers;
    /// @brief list of connected devices, those unrecognizable devices will be
    /// categoried into 'unknown devices'
    LinkedList<Device&>               m_deviceList[DeviceTypeUnknown + 1];
    /// @brief the amount of connected devices
    u32                               m_connectedDeviceCount;
    /// @brief auto connect and load new but unneccssary devices (bluetooth,
    /// external hard drive, etc.) when they are available if set to true
    bool                              m_isAutoConnect;
};