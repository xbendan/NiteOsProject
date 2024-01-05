#include <drivers/comm/spec.h>
#include <siberix/dvc/device.h>
#include <xtra-io/writer.h>

namespace Kern::Hal::Impls {
    class SerialPortDevice
      : public IDevice
      , public Io::TextWriter
    {
    public:
        SerialPortDevice()
          : IDevice("Serial Port Device", DeviceType::Firmware)
        {
        }
    };
}