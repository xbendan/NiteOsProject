#include <Device/Device.h>

Device::Device(const char *name, DeviceBus bus, DeviceType type)
  : m_Name(name),
    m_DeviceBus(bus),
    m_Type(type)
{

}