#include <mixins/types.h>

namespace mixins::math {
    template <size_t _S, size_t _E, size_t _M>
        requires(_S <= 1 && _E >= 1 && _M >= 1 && (_S + _E + _M) <= 64)
    class FloatingPoint final
    {
    public:
        using Sign     = _S;
        using Exponent = _E;
        using Mantissa = _M;

    private:
        uint64_t m_bits;
    };

    template <typename>
    union FloatingPointBits;

    template <>
    union FloatingPointBits<__float128>
    {
        static constexpr size_t Mantissa = 112;
        static constexpr size_t Exponent = 15;
        static constexpr size_t Sign     = 1;
    };

    template <>
    union FloatingPointBits<double>
    {
        static constexpr size_t Mantissa = 52;
        static constexpr size_t Exponent = 11;
        static constexpr size_t Sign     = 1;
        struct
        {
            uint64_t mantissa : Mantissa;
            uint64_t exponent : Exponent;
            uint64_t sign : Sign;
        };
        double d;
    };

    template <>
    union FloatingPointBits<float>
    {
        static constexpr size_t Mantissa = 23;
        static constexpr size_t Exponent = 8;
        static constexpr size_t Sign     = 1;
        struct
        {
            uint32_t mantissa : Mantissa;
            uint32_t exponent : Exponent;
            uint32_t sign : Sign;
        };
        float f;
    };
}