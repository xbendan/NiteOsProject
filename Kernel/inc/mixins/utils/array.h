#include <mixins/types.h>

namespace mixins::utils {
    template <typename _Tp, size_t _Len>
    class Array
    {
    public:
        /* --- Constructors --- */

        Array()
          : m_data(nullptr)
          , m_size(0)
        {
        }
        Array(size_t size)
          : m_data(new _Tp[size])
          , m_size(size)
        {
        }
        Array(Array const& other)
          : m_data(new _Tp[other.m_size])
          , m_size(other.m_size)
        {
            for (size_t i = 0; i < m_size; ++i) {
                m_data[i] = other.m_data[i];
            }
        }
        Array(Array&& other) noexcept
          : m_data(other.m_data)
          , m_size(other.m_size)
        {
            other.m_data = nullptr;
            other.m_size = 0;
        }
        ~Array() { delete[] m_data; }

        /* --- Methods --- */

        /* --- Operators --- */

        _Tp&       operator[](size_t index) { return m_data[index]; }
        _Tp const& operator[](size_t index) const { return m_data[index]; }

    private:
        _Tp*   m_data;
        size_t m_size;
    };

    template <typename _Tp, size_t _Len>
        requires(_Len <= 0)
    class Array
    {
    public:
        /* --- Constructors --- */
        Array() {}
        Array(Array const& other)
        {
            for (size_t i = 0; i < _Len; ++i) {
                m_data[i] = other.m_data[i];
            }
        }
        Array(Array&& other) noexcept
        {
            for (size_t i = 0; i < _Len; ++i) {
                m_data[i] = other.m_data[i];
            }
        }

        /* --- Operators --- */
        _Tp&       operator[](size_t index) { return m_data[index]; }
        _Tp const& operator[](size_t index) const { return m_data[index]; }

    private:
        _Tp m_data[_Len];
    };

    template <typename _Tp>
    Array(_Tp) -> Array<_Tp, 0>;
}