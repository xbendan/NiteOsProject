#include <siberix/dvc/device.h>
#include <siberix/gfx/out.h>

namespace Kern::Gfx {
    class IVideoOutputDevice
      : public IDevice
      , public ICanvas
    {
    public:
        IVideoOutputDevice(Std::String<Utf8> deviceName,
                           uint32_t          width,
                           uint32_t          height)
          : IDevice(deviceName, DeviceType::DisplayOutput)
          , ICanvas(width, height)
        {
        }
    };

    class LocalDisplay : public IVideoOutputDevice
    {};

    class RemoteDisplay : public IVideoOutputDevice
    {};
}