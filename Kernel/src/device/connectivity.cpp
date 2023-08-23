#include <common/logger.h>
#include <siberix/device/connectivity.h>

DeviceConnectivity::DeviceConnectivity() {}

DeviceConnectivity::~DeviceConnectivity() {}

void DeviceConnectivity::registerDevice(Device* device) {
    if (device == nullptr) {
        return;
    }
    if (!device->isWorking()) {
        Logger::getLogger("device").warn(
            "The device [%s] might not working properly. Make sure there isn't missing components.",
            device->getName());
    }
    m_deviceList[static_cast<u8>(device->getType())].add(*device);
    m_connectedDeviceCount++;
}

void DeviceConnectivity::unregisterDevice(Device* device) {
    if (device == nullptr) {
        return;
    }
    m_deviceList[static_cast<u8>(device->getType())].remove(*device);
    m_connectedDeviceCount--;
}

Device* DeviceConnectivity::findDevice(const char* name) {
    for (u8 i = 0; i < static_cast<u8>(DeviceType::Unknown) + 1; i++) {
        ListNode<Device&>* node = m_deviceList[i].first();
        while (node != nullptr) {
            if (strcmp(node->obj.getName(), name) == 0) {
                return &node->obj;
            }
            node = node->next;
        }
    }
    return nullptr;
}

LinkedList<Device&>& DeviceConnectivity::enumerateDevice(DeviceType type) {
    return m_deviceList[static_cast<u8>(type)];
}