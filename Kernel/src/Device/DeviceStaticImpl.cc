#include <Device/Device.h>

Device Device::GetDevice(const char *str)
{

}

Device Device::GetDevice(uint64_t id)
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

DeviceProvider Device::GetProvider(DeviceBus bus)
{

}

void Device::EnumerateDevice(DeviceType type)
{

}