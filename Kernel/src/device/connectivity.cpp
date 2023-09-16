#include <common/logger.h>
#include <siberix/device/connectivity.h>

DeviceConnectivity::DeviceConnectivity()
{
    Logger::getLogger("device").success("Initialized device connectivity.\n");
}

DeviceConnectivity::~DeviceConnectivity() {}

u64
DeviceConnectivity::registerDevice(Device* device)
{
    if (device == nullptr) {
        Logger::getLogger("device").warn(
          "Cannot register device with a null pointer.\n");
        return 0;
    }
    if (!device->isWorking()) {
        Logger::getLogger("device").warn(
          "The device [%s] might not working properly. Make sure there isn't "
          "missing components.\n",
          device->getName());
    }
    m_deviceList[static_cast<u8>(device->getType())].add(*device);

    Logger::getLogger("device").success(
      "Device [%s] registered with device id [%u].\n",
      device->getName(),
      m_connectedDeviceCount);

    return m_connectedDeviceCount++;
}

void
DeviceConnectivity::unregisterDevice(Device* device)
{
    if (device == nullptr) {
        return;
    }
    m_deviceList[static_cast<u8>(device->getType())].remove(*device);
    m_connectedDeviceCount--;
}

Device*
DeviceConnectivity::findDevice(const char* name)
{
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

LinkedList<Device&>&
DeviceConnectivity::enumerateDevice(DeviceType type)
{
    return m_deviceList[static_cast<u8>(type)];
}