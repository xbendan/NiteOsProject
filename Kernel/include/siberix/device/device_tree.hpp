#include <utils/linked_list.h>

#include <siberix/device/device.hpp>

class DeviceTree {
public:
    Device*              getDevice(const char* str);
    Device*              getDevice(u64 id);
    void                 registerDevice(Device& device);
    void                 unregisterDevice(Device& device);
    LinkedList<Device&>& enumerateDevice(DeviceType type);

private:
    LinkedList<Device&> deviceList[DeviceTypeUnknown + 1];
    int                 count;
};