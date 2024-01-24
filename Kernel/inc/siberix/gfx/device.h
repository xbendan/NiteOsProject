#include <siberix/dvc/device.h>
#include <siberix/gfx/attr.h>
#include <siberix/gfx/canvas.h>

namespace Kern::Gfx {
    class IVideoOutputDevice
      : public Device
      , public ICanvas
    {
    public:
        IVideoOutputDevice(Std::String<Utf8> deviceName,
                           uint32_t          width,
                           uint32_t          height,
                           uint64_t          capabilties = 0)
          : Device(deviceName, DeviceType::DisplayOutput)
          , ICanvas(width, height)
          , m_capabilities(capabilties)
        {
        }

        bool isSupported(DisplayCapability capability)
        {
            return m_capabilities & capability;
        }

    private:
        uint64_t m_capabilities;
    };

    class LocalDisplay : public IVideoOutputDevice
    {
    public:
        LocalDisplay(Std::String<Utf8> deviceName,
                     uint32_t          width,
                     uint32_t          height,
                     uint64_t          capabilties = 0)
          : IVideoOutputDevice(deviceName, width, height)
        {
        }
        ~LocalDisplay() = default;
    };

    class RemoteDisplay : public IVideoOutputDevice
    {};
}