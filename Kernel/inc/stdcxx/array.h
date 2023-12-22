#include <stdcxx/types.h>

namespace Std {
    template <typename T, uint64_t Capacity>
        requires(Capacity > 0)
    class Array
    {
        T _buffer[Capacity];

        T&       operator[](uint64_t index) { return _buffer[index]; }
        T const& operator[](uint64_t index) const { return _buffer[index]; }
        uint64_t   length() const { return Capacity; }
    };

    template <typename T>
    class Array<T, 0>
    {
    public:
        Array(uint64_t length)
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

        T&       operator[](uint64_t index) { return _buffer[index]; }
        T const& operator[](uint64_t index) const { return _buffer[index]; }
        uint64_t   length() const { return _length; }

    private:
        T*     _buffer;
        uint64_t _length;
    };
}