#include <drivers/ps2/spec.h>
#include <siberix/dvc/device.h>

namespace Kern::Hal::Impls {
    class PS2LegacyControllerDevice : public Device
    {
    public:
        PS2LegacyControllerDevice();
        ~PS2LegacyControllerDevice();
    };
}