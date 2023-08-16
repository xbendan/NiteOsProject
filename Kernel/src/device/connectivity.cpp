#include <common/logger.h>
#include <siberix/device/connectivity.h>

DeviceConnectivity::DeviceConnectivity() {}

DeviceConnectivity::~DeviceConnectivity() {}

bool DeviceConnectivity::install(Device& device) {
    if (!device.isWorking()) {
        Logger::getLogger("device").warn(
            "The device [%s] might not working properly. Make sure there isn't missing components.",
            device.getName());
        return false;
    }
    m_deviceList[static_cast<u8>(device.getType())].add(device);
    m_connectedDeviceCount++;

    return true;
}

bool DeviceConnectivity::uninstall(Device& device) { return false; }