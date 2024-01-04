#include <stdcxx/iterator.h>
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
        Array(T buffer[], int length)
        {
            if (m_buffer != nullptr)
                delete[] m_buffer;

            m_buffer = buffer;
            m_length = length;
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
        void operator=(T* buffer)
        {
            if (m_buffer != nullptr)
                delete[] m_buffer;

            m_buffer = buffer;
        }

        ArrayIterator begin() { return ArrayIterator(*this, 0); }
        ArrayIterator end() { return ArrayIterator(*this, m_length); }
        uint64_t      length() const { return m_length; }

        class ArrayIterator : public Iterator<T>
        {
        public:
            ArrayIterator(Array<T>& array, uint64_t index)
              : m_array(array)
              , m_index(index)
            {
            }

            ArrayIterator& operator++() override
            {
                ++m_index;
                return *this;
            }
            ArrayIterator& operator--() override
            {
                --m_index;
                return *this;
            }
            T&   operator*() override { return m_array[m_index]; }
            bool operator==(Iterator<T>& other) override
            {
                return m_array[m_index] == other.operator*();
            }

        private:
            Array<T>& m_array;
            uint64_t  m_index;
        };

    private:
        T*       m_buffer;
        uint64_t m_length;
    };

    template <typename T, uint64_t Capacity>
    class StaticArray
    {
    public:
        constexpr StaticArray() = default;
        constexpr StaticArray(const T (&newArray)[Capacity])
        {
            for (uint64_t i = 0; i < Capacity; ++i) {
                m_buffer[i] = newArray[i];
            }
        }

        T&       operator[](uint64_t index) { return m_buffer[index]; }
        T const& operator[](uint64_t index) const { return m_buffer[index]; }
        void     operator=(StaticArray const& other) = delete;
        void     operator=(StaticArray&& other)      = delete;
        void     operator=(const T (&newArray)[Capacity])
        {
            for (uint64_t i = 0; i < Capacity; ++i) {
                m_buffer[i] = newArray[i];
            }
        }

        uint64_t length() const { return Capacity; }

    private:
        T m_buffer[Capacity];
    };
}

using Bytes = Std::Array<uint8_t>;