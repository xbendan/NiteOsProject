#include <siberix/device/device_tree.hpp>

Device* DeviceTree::getDevice(const char* str) {}

Device* DeviceTree::getDevice(u64 id) {}

void DeviceTree::registerDevice(Device& device) {}

void DeviceTree::unregisterDevice(Device& device) {}

LinkedList<Device&>& DeviceTree::enumerateDevice(DeviceType type) {
    return deviceList[type];
}