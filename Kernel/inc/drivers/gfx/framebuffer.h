#include <siberix/gfx/device.h>
#include <stdcxx/buffer.h>

namespace Kern::Video {
    using namespace Gfx;
    class FramebufferDisplay : public LocalDisplay
    {
    public:
        FramebufferDisplay(uint32_t width, uint32_t height)
          : LocalDisplay("Framebuffer Buffered Display", width, height)
        {
        }

    private:
        Std::Buffer<Pixel<8>> m_buf;
    };
}