#include <stdcxx/types.h>

namespace Std {
    template <typename T, UInt64 Capacity>
        requires(Capacity > 0)
    class Array
    {
        T _buffer[Capacity];

        T&       operator[](UInt64 index) { return _buffer[index]; }
        T const& operator[](UInt64 index) const { return _buffer[index]; }
        UInt64   length() const { return Capacity; }
    };
}