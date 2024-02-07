#pragma once

#include <initializer_list>
#include <siberix/dvc/device.h>
#include <siberix/svc/svc-host.h>
#include <stdcxx/array-list.h>
#include <stdcxx/linked_list.h>
#include <stdcxx/opt.h>

namespace Kern {
    class IDeviceEnumerator : public Device
    {
    public:
        IDeviceEnumerator(Std::String<Utf8> name)
          : Device(name, DeviceType::SoftwareDevice)
        {
        }

        virtual Std::ArrayList<Device*>* scan() = 0;
    };

    template <typename T>
    concept Devices =
      Std::isBaseOf<Device, typename Std::RemovePointer<T>::Type>::Value;

    class DeviceConnectivity : Svc::ISvcHost
    {
    public:
        DeviceConnectivity()
          : Svc::ISvcHost("KERN.DEVICE")
        {
        }
        // template <Devices... Args>
        // DeviceConnectivity(Args... args)
        //   : Svc::ISvcHost("KERN.DEVICE")
        // {
        //     for (Device* device : { args... }) {
        //         registerDevice(device);
        //     }
        // }
        DeviceConnectivity(std::initializer_list<Device*> initials)
          : Svc::ISvcHost("KERN.DEVICE")
        {
            for (Device* dev : initials) {
                registerDevice(dev);
            }
        }
        ~DeviceConnectivity() = default;

        Opt<Device>                         findDevice(Std::String<Utf8> name);
        Opt<Device>                         findDevice(Std::UUID uuid);
        Opt<Device>                         findDevice(uint64_t deviceId);
        void                                registerDevice(Device* device);
        void                                unregisterDevice(Device* device);
        Std::LinkedList<Device>*            getAllDevices();
        uint64_t                            count();
        Std::LinkedList<IDeviceEnumerator>* getAllEnumerators();

        void onLoad() override;

    private:
        Std::LinkedList<Device>             m_devices;
        Std::LinkedList<IDeviceEnumerator*> m_enumerators;
        bool                                m_isAutoConnect;
    };
}