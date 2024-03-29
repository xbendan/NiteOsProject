#include <stdcxx/types.h>
#include <xtra-math/vec.h>

namespace Kern::Gfx {
    template <uint8_t BitWidth>
    struct Pixel
    {
        uint16_t _red : BitWidth;
        uint16_t _green : BitWidth;
        uint16_t _blue : BitWidth;
    };

    using Pixel8  = Pixel<8>;
    using Pixel10 = Pixel<10>;
    using Pixel12 = Pixel<12>;

    class ICanvas
    {
    public:
        ICanvas(uint32_t width, uint32_t height)
          : m_width(width)
          , m_height(height)
        {
        }

        virtual void setWidth(uint32_t width) { m_width = width; }
        virtual void setHeight(uint32_t height) { m_height = height; }
        uint32_t     getWidth() const { return m_width; }
        uint32_t     getHeight() const { return m_height; }

        virtual void drawPixel(Vector<uint32_t, 2> pos, Pixel8 pixel);

    protected:
        uint32_t m_width;
        uint32_t m_height;
    };
}