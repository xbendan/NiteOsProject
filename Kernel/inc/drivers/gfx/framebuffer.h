#include <siberix/gfx/device.h>
#include <stdcxx/buffer.h>

namespace Kern::Video {
    class FramebufferVODevice : public IVideoOutputDevice
    {
    public:
        FramebufferVODevice(uint32_t width, uint32_t height) {}

    private:
        Std::Buffered<Pixel<8>> m_buf;
    };
}