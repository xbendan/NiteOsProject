#include <common/logger.h>

#include <siberix/device/connectivity.hpp>

DeviceConnectivity::DeviceConnectivity() {}

DeviceConnectivity::~DeviceConnectivity() {}

DeviceConnectivity::install(Device& device) {
    if (!device.isWorking()) {
        Logger::getLogger("device").warn(
            "The device installing might not working properly.");
    }
    m_deviceList[device.getType()].add(device);
    m_connectedDeviceCount++;
}

DeviceConnectivity::uninstall(Device& device) {}