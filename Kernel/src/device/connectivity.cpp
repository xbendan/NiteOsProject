#include <common/logger.h>
#include <siberix/device/connectivity.h>

DeviceConnectivity::DeviceConnectivity() {}

DeviceConnectivity::~DeviceConnectivity() {}

bool DeviceConnectivity::install(Device& device) {
    if (!device.isWorking()) {
        Logger::getLogger("device").warn(
            "The device installing might not working properly.");
    }
    m_deviceList[static_cast<u8>(device.getType())].add(device);
    m_connectedDeviceCount++;
}

bool DeviceConnectivity::uninstall(Device& device) {}