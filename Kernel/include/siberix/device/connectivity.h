#pragma once

#include <siberix/device/device.h>
#include <utils/linked_list.h>

class DeviceConnectivity
{
public:
    DeviceConnectivity();
    ~DeviceConnectivity();

    Device*                     getDevice(u64 deviceId);
    u64                         registerDevice(Device* device);
    void                        unregisterDevice(Device* device);
    Device*                     findDevice(const char* str);
    utils::LinkedList<Device&>& enumerateDevice(DeviceType type);
    u32                         count();
    bool                        isAutoConnect();
    void                        setAutoConnect(bool autoConnect);

private:
    /// @brief list of connected devices, those unrecognizable devices will be
    /// categoried into 'unknown devices'
    utils::LinkedList<Device&>
         m_deviceList[static_cast<u64>(DeviceType::Unknown) + 1];
    /// @brief the amount of connected devices
    u32  m_connectedDeviceCount;
    /// @brief auto connect and load new but unneccssary devices (bluetooth,
    /// external hard drive, etc.) when they are available if set to true
    bool m_isAutoConnect;

protected:
    friend Device;
};