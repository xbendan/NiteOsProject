#include <stdcxx/types.h>

namespace xtra::Gfx {
    template <uint8_t BitWidth>
        requires(BitWidth == 8 || BitWidth == 10 || BitWidth == 12 ||
                 BitWidth == 16)
    union Color
    {
        struct
        {
            uint16_t _r : BitWidth;
            uint16_t _g : BitWidth;
            uint16_t _b : BitWidth;
            uint16_t _a : BitWidth;
        };
        uint64_t _data;

        Color()
          : _data(0)
        {
        }
        Color(uint64_t data)
          : _data(data)
        {
        }
        Color(uint64_t r, uint64_t g, uint64_t b, uint64_t a)
          : _r(r)
          , _g(g)
          , _b(b)
          , _a(a)
        {
        }
        Color(const Color<BitWidth>& other)
          : _data(other._data)
        {
        }
    };
}