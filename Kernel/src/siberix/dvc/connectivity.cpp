#include <siberix/dvc/connectivity.h>

namespace Kern {
    void DeviceConnectivity::onLoad() {}

    Device* findDevice(Std::String<Utf8> name) {}

    Device*                              findDevice(Std::UUID uuid);
    Device*                              findDevice(uint64_t deviceId);
    void                                 registerDevice(Device* device);
    void                                 unregisterDevice(Device* device);
    Std::LinkedList<Device*>*            getAllDevices();
    uint64_t                             count();
    Std::LinkedList<IDeviceEnumerator*>* getAllEnumerators();
}