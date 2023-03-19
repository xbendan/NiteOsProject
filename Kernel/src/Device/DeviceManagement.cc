#include <Device/Device.h>
#include <Utils/ArrayList.h>

namespace DeviceManagement
{
    SizedArrayList<DeviceProvider *, 4> g_DeviceProviders;
    SizedArrayList<LinkedList<Device *>, DeviceType::DeviceTypeUnknown + 1> g_DeviceLists;
} // namespace DeviceManagement