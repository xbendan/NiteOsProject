#include <stdcxx/types.h>

namespace Std {
    // template <typename T, uint64_t Capacity>
    //     requires(Capacity > 0)
    // class Array
    // {
    // public:
    //     constexpr Array() = default;
    //     constexpr Array(const T (&newArray)[Capacity])
    //     {
    //         for (uint64_t i = 0; i < Capacity; ++i) {
    //             m_buffer[i] = newArray[i];
    //         }
    //     }

    //     T&       operator[](uint64_t index) { return m_buffer[index]; }
    //     T const& operator[](uint64_t index) const { return m_buffer[index]; }
    //     void     operator=(Array const& other) = delete;
    //     void     operator=(Array&& other)      = delete;
    //     void     operator=(const T (&newArray)[Capacity])
    //     {
    //         for (uint64_t i = 0; i < Capacity; ++i) {
    //             m_buffer[i] = newArray[i];
    //         }
    //     }

    //     uint64_t length() const { return Capacity; }

    // private:
    //     T m_buffer[Capacity];
    // };

    template <typename T>
    class Array
    {
    public:
        Array() = default;
        Array(uint64_t length)
        {
            if (m_buffer != nullptr)
                delete[] m_buffer;

            m_buffer = new T[length];
            m_length = length;
        }
        Array(Array&& other)
        {
            m_buffer = other.m_buffer;
            m_length = other.m_length;

            other.m_buffer = nullptr;
            other.m_length = 0;
        }
        ~Array()
        {
            if (m_buffer != nullptr)
                delete[] m_buffer;
        }

        T&   operator[](uint64_t index) { return m_buffer[index]; }
        void operator=(Array const& other)
        {
            if (m_buffer != nullptr)
                delete[] m_buffer;

            m_buffer = new T[other.m_length];
            m_length = other.m_length;

            for (uint64_t i = 0; i < m_length; ++i) {
                m_buffer[i] = other.m_buffer[i];
            }
        }
        void operator=(Array&& other)
        {
            if (m_buffer != nullptr)
                delete[] m_buffer;

            m_buffer = other.m_buffer;
            m_length = other.m_length;

            other.m_buffer = nullptr;
            other.m_length = 0;
        }
        uint64_t length() const { return m_length; }

    private:
        T*       m_buffer;
        uint64_t m_length;
    };
}