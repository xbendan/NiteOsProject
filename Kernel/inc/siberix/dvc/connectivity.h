#pragma once

#include <siberix/dvc/device.h>
#include <siberix/svc/svc-host.h>
#include <stdcxx/array-list.h>
#include <stdcxx/linked-list.h>

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
      Std::IsBaseOf<Device, typename Std::RemovePointer<T>::Type>::Value;

    class DeviceConnectivity : Svc::ISvcHost
    {
    public:
        DeviceConnectivity()
          : Svc::ISvcHost("KERN.DEVICE")
        {
        }
        template <Devices... Args>
        DeviceConnectivity(Args... args)
          : Svc::ISvcHost("KERN.DEVICE")
        {
            for (auto& device : { args... }) {
                registerDevice(device);
            }
        }
        ~DeviceConnectivity() = default;

        Device*                              findDevice(Std::String<Utf8> name);
        Device*                              findDevice(Std::UUID uuid);
        Device*                              findDevice(uint64_t deviceId);
        void                                 registerDevice(Device* device);
        void                                 unregisterDevice(Device* device);
        Std::LinkedList<Device*>*            getAllDevices();
        uint64_t                             count();
        Std::LinkedList<IDeviceEnumerator*>* getAllEnumerators();

        void onLoad() override;
        void onEnable() override;
        void onDisable() override;

    private:
        Std::LinkedList<Device*>            m_devices;
        Std::LinkedList<IDeviceEnumerator*> m_enumerators;
        bool                                m_isAutoConnect;
    };
}