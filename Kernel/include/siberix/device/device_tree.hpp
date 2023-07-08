#include <siberix/device/device.hpp>
#include <utils/linked_list.h>

class DeviceTree {
public:
    Device* getDevice(const char* str);
    Device* getDevice(u64 id);
    void registerDevice(Device& device);
    void unregisterDevice(Device& device);
    void enumerateDevice(DeviceType type);

private:
    LinkedList<Device&> deviceList[DeviceTypeUnknown + 1];
    int count;
};