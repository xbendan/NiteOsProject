#include <drivers/usb/xhci/spec.h>

namespace Kern::Hal::Impls {
    class XhciControllerDevice : public IPeriCompDevice
    {
    public:
        XhciControllerDevice(PCIInfo& info);
        ~XhciControllerDevice();
    };
}