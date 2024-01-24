#include <drivers/comm/spec.h>
#include <siberix/dvc/device.h>
#include <xtra-io/writer.h>

namespace Kern::Hal::Impls {
    class SerialPortDevice
      : public Device
      , public TextWriter
    {
    public:
        SerialPortDevice()
          : Device("Serial Port Device", DeviceType::Firmware)
        {
        }

        void write(char c) override;
        void write(Std::String<Utf8> str) override;
    };
}