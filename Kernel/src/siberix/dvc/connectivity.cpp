#include <siberix/dvc/connectivity.h>

namespace Kern {
    void DeviceConnectivity::onLoad() {}

    Opt<Device> DeviceConnectivity::findDevice(Std::String<Utf8> name)
    {
        return m_devices.stream()
          ->filter([name](Device* device) -> bool {
              return device->getName() == name;
          })
          .findFirst();
    }

    Opt<Device> DeviceConnectivity::findDevice(Std::UUID uuid)
    {
        return m_devices.stream()
          ->filter([uuid](Device* device) -> bool {
              return device->getUniqueId() == uuid;
          })
          .findFirst();
    }

    Opt<Device> DeviceConnectivity::findDevice(uint64_t deviceId)
    {
        return Opt<Device>::empty();
        // return m_devices.stream()
        //   ->filter([&](Device* device) -> bool {
        //       return device->getId() == deviceId;
        //   })
        //   .findFirst();
    }

    void DeviceConnectivity::registerDevice(Device* device) {}

    void DeviceConnectivity::unregisterDevice(Device* device) {}

    Std::LinkedList<Device>* DeviceConnectivity::getAllDevices()
    {
        return &m_devices;
    }

    uint64_t DeviceConnectivity::count()
    {
        return m_devices.size();
    }

    Std::LinkedList<IDeviceEnumerator>* DeviceConnectivity::getAllEnumerators()
    {
    }
}