#include <common/logger.h>
#include <siberix/core/runtimes.h>
#include <siberix/device/connectivity.h>
#include <siberix/device/device.h>

const char* _unknownDeviceName = "<Unknown Device>";

Device::Device(const char* _name, DeviceBus _bus, DeviceType _type)
  : m_name(_name)
  , m_bus(_bus)
  , m_type(_type)
{
    // m_deviceId = siberix()->getConnectivity()->registerDevice(this);

    m_flags |= DeviceFlags::DeviceInstalled;
    m_flags |= DeviceFlags::DeviceInitialized;
}

Device::Device(const char* _name)
  : m_name(_name)
  , m_bus(DeviceBus::Unknown)
  , m_type(DeviceType::Unknown)
{
    // m_deviceId = siberix()->getConnectivity().registerDevice(this);
}

Device::Device(DeviceBus bus, DeviceType type)
  : m_name(_unknownDeviceName)
  , m_bus(bus)
  , m_type(type)
{
    // m_deviceId = siberix()->getConnectivity().registerDevice(this);
}

Device::~Device() {}