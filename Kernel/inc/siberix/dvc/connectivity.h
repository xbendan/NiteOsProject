#pragma once

#include <siberix/dvc/device.h>
#include <siberix/svc/svc-host.h>
#include <stdcxx/linked-list.h>

namespace Kern {
    class IDeviceEnumerator : public IDevice
    {
    public:
        IDeviceEnumerator(Std::String<Utf8> name)
          : IDevice(name, DeviceType::SoftwareDevice)
        {
        }

        virtual Std::Array<IDevice*>* scan() = 0;
    };

    class DeviceConnectivity : Svc::ISvcHost
    {
    public:
        DeviceConnectivity()
          : Svc::ISvcHost("KERN.DEVICE")
        {
        }
        DeviceConnectivity(Std::Array<IDevice*>* devices)
          : Svc::ISvcHost("KERN.DEVICE")
        {
        }
        ~DeviceConnectivity() = default;

        IDevice*                             findDevice(Std::String<Utf8> name);
        IDevice*                             findDevice(Std::UUID uuid);
        IDevice*                             findDevice(uint64_t deviceId);
        void                                 registerDevice(IDevice* device);
        void                                 unregisterDevice(IDevice* device);
        Std::LinkedList<IDevice*>*           getAllDevices();
        uint64_t                             count();
        Std::LinkedList<IDeviceEnumerator*>* getAllEnumerators();

        void onLoad() override;
        void onEnable() override;
        void onDisable() override;

    private:
        Std::LinkedList<IDevice*>           m_devices;
        Std::LinkedList<IDeviceEnumerator*> m_enumerators;
        bool                                m_isAutoConnect;
    };
}