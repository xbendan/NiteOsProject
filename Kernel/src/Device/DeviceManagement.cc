#include <Device/Device.h>
#include <Utils/ArrayList.h>

SizedArrayList<DeviceProvider *, 4> g_DeviceProviders;
SizedArrayList<LinkedList<Device *>, DeviceType::DeviceTypeUnknown + 1> g_DeviceLists;