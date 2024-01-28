#include <drivers/smbios/spec.h>
#include <siberix/dvc/device.h>

namespace Kern::Hal::Impls {
    using namespace Specs;

    class SmbiosDevice : public Device
    {
    public:
        SmbiosDevice();
        ~SmbiosDevice() = default;

    private:
        uint8_t m_majorVer;
        uint8_t m_minorVer;
        void*   m_smbiosInfo;
    };
}