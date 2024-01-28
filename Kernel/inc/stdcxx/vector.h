#include <stdcxx/move.h>
#include <stdcxx/types.h>

#define DEFAULT_VECTOR_CAPACITY 8
#define DEFAULT_VECTOR_GROWTH_FACTOR 2

namespace Std {

    template <typename T>
    class Vector
    {
    public:
        Vector()
          : m_buffer(new T[DEFAULT_VECTOR_CAPACITY])
          , m_capacity(DEFAULT_VECTOR_CAPACITY)
          , m_length(0)
          , m_factor(DEFAULT_VECTOR_GROWTH_FACTOR)
        {
        }
        Vector(uint32_t capacity, float factor = DEFAULT_VECTOR_GROWTH_FACTOR)
          : m_buffer(new T[capacity])
          , m_capacity(capacity)
          , m_length(0)
        {
        }
        Vector(Vector<T> const& other)
          : m_buffer(other.m_buffer)
          , m_capacity(other.m_capacity)
          , m_length(other.m_length)
          , m_factor(other.m_factor)
        {
        }
        Vector(Vector<T>&& other)
          : m_buffer(other.m_buffer)
          , m_capacity(other.m_capacity)
          , m_length(other.m_length)
          , m_factor(other.m_factor)
        {
            other.m_buffer   = nullptr;
            other.m_capacity = 0;
            other.m_length   = 0;
        }
        ~Vector()
        {
            delete[] m_buffer;
            m_length = 0;
        }

        Vector<T>& operator=(Vector<T> const& other)
        {
            m_buffer   = other.m_buffer;
            m_capacity = other.m_capacity;
            m_length   = other.m_length;
            m_factor   = other.m_factor;
            return *this;
        }

        Vector<T>& operator=(Vector<T>&& other)
        {
            m_buffer         = other.m_buffer;
            m_capacity       = other.m_capacity;
            m_length         = other.m_length;
            m_factor         = other.m_factor;
            other.m_buffer   = nullptr;
            other.m_capacity = 0;
            other.m_length   = 0;
            return *this;
        }

        /* --- Collection --- */

        void add(T const& value) { pushBack(value); }

        void add(T&& value) { pushBack(Std::Move(value)); }

        bool contains(T const& value) const
        {
            for (uint32_t i = 0; i < m_length; i++) {
                if (m_buffer[i] == value) {
                    return true;
                }
            }
            return false;
        }

        void remove() {}

        /* --- Capacity --- */

        bool isEmpty() const { return m_length == 0; }

        uint32_t size() const { return m_length; }

        uint32_t maxSize() const { return (uint32_t)-1; }

        uint32_t capacity() const { return m_capacity; }

        bool reserve(uint32_t newSize)
        {
            if (newSize <= m_capacity) {
                return false;
            }
            T* newBuffer = new T[newSize];
            // do copy
            delete[] m_buffer;
            m_buffer = newBuffer;
        }

        bool shrink()
        {
            if (m_length == m_capacity) {
                return false;
            }
            T* newBuffer = new T[m_length];
            // do copy
            delete[] m_buffer;
            m_buffer = newBuffer;
        }

        /* --- Modifiers --- */

        void clear() { m_length = 0; }

        void insert(uint32_t index, T const& value)
        {
            if (index >= m_length) {
                return;
            }
            if (m_length == m_capacity) {
                reserve(m_capacity * m_factor);
            }

            // do insert
            new (&m_buffer[index]) T(Std::Forward(value));
            m_length++;
        }

        void pushFront(T const& value) { insert(0, value); }

        void pushFront(T&& value) { insert(0, Std::Move(value)); }

        void pushBack(T const& value) { insert(m_length, value); }

        void pushBack(T&& value) { insert(m_length, Std::Move(value)); }

        void popBack()
        {
            if (m_length == 0) {
                return;
            }
            m_length--;
        }

        /* --- Element access --- */

        T& at(uint32_t index)
        {
            if (index >= m_length) {
                // panic
            }
            return m_buffer[index];
        }

        T& operator[](uint32_t index) { return m_buffer[index]; }

        T& front() { return m_buffer[0]; }

        T& back() { return m_buffer[m_length - 1]; }

        T* data() { return m_buffer; }

    private:
        T*       m_buffer;
        uint32_t m_capacity;
        uint32_t m_length;
        float    m_factor;
    };
}