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

    template <typename T>
    class Array<T, 0>
    {
    public:
        Array(UInt64 length)
        {
            if (_buffer != nullptr)
                delete[] _buffer;

            _buffer = new T[length];
            _length = length;
        }
        ~Array()
        {
            if (_buffer != nullptr)
                delete[] _buffer;
        }

        T&       operator[](UInt64 index) { return _buffer[index]; }
        T const& operator[](UInt64 index) const { return _buffer[index]; }
        UInt64   length() const { return _length; }

    private:
        T*     _buffer;
        UInt64 _length;
    };
}