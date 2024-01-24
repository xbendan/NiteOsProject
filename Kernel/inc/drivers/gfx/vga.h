#include <siberix/gfx/device.h>

namespace Kern::Hal::Impls {
    using namespace Gfx;
    class VGATextDisplay : public LocalDisplay
    {
    public:
        VGATextDisplay()
          : LocalDisplay(
              "VGA Text Mode Display",
              80,
              25,
              (DisplayCapability::ColorVGA | DisplayCapability::TextFormat))
          , m_buf(reinterpret_cast<uint16_t*>(0xB8000))
        {
        }

    private:
        uint16_t* m_buf;
    };
}