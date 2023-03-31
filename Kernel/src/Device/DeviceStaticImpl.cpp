#include <Device/Device.h>

uint64_t Device::allocatedId = 0;
SizedArrayList<DeviceProvider *, 4> Device::g_DeviceProviders;
SizedArrayList<LinkedList<Device *>, DeviceType::DeviceTypeUnknown + 1> Device::g_DeviceLists;

Device *Device::GetDevice(const char *str)
{

}

Device *Device::GetDevice(uint64_t id)
{

}

void Device::RegisterDevice(Device *device)
{
    if (device->m_DeviceId)
    {
        return;
    }

    LinkedList<Device *> *deviceList = &g_DeviceLists[device->m_Type];
    if (deviceList != nullptr)
    {
        deviceList->Add(device);
        device->m_DeviceId = allocatedId++;
    }
}

DeviceProvider *Device::GetProvider(DeviceBus bus)
{

}

void Device::EnumerateDevice(DeviceType type)
{

}