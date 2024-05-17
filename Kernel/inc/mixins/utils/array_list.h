#pragma once

#include <mixins/utils/collection.h>

#define DEFAULT_ARRAY_LIST_CAPACITY 8
#define DEFAULT_ARRAY_LIST_GROWTH_FACTOR 2

namespace mixins::utils {
    template <typename _Tp>
        requires(!std::isTrivial<_Tp>::value)
    class ArrayList : IList<_Tp>
    {
    public:
        /* --- Constructors --- */
        ArrayList()
          : ArrayList(DEFAULT_ARRAY_LIST_CAPACITY)
        {
        }
        ArrayList(uint64_t capacity, bool increment = true)
          : m_pointers(new _Tp*[capacity])
          , m_length(0)
          , m_capacity(capacity)
          , m_increment(increment)
        {
        }
        ArrayList(ArrayList const& other)
          : m_pointers(new _Tp*[other.m_capacity])
          , m_length(other.m_length)
          , m_capacity(other.m_capacity)
          , m_increment(other.m_increment)
        {
            for (uint64_t i = 0; i < m_length; ++i) {
                (*m_pointers)[i] = (*other.m_pointers)[i];
            }
        }
        ArrayList(ArrayList&& other) noexcept
          : m_pointers(other.m_pointers)
          , m_length(other.m_length)
          , m_capacity(other.m_capacity)
          , m_increment(other.m_increment)
        {
            other.m_pointers = nullptr;
            other.m_length   = 0;
            other.m_capacity = 0;
        }
        ~ArrayList()
        {
            for (uint64_t i = 0; i < m_length; ++i) {
                delete m_pointers[i];
            }
            delete[] m_pointers;
        }

        /* --- Methods --- */
        void add(_Tp* value)
        {
            if (m_length == m_capacity && m_increment) {
                if (m_increment) {
                    m_capacity *= DEFAULT_ARRAY_LIST_GROWTH_FACTOR;
                }
                _Tp** newPointers = new _Tp*[m_capacity];
                for (uint64_t i = 0; i < m_length; ++i) {
                    newPointers[i] = m_pointers[i];
                }
                delete[] m_pointers;
                m_pointers = newPointers;
            }
            m_pointers[m_length++] = value;
        }

        void add(_Tp const& value) { add(&value); }

        void remove(uint64_t index)
        {
            if (index >= m_length) {
                return;
            }
            delete m_pointers[index];
            for (uint64_t i = index; i < m_length - 1; ++i) {
                m_pointers[i] = m_pointers[i + 1];
            }
            --m_length;
        }

        void remove(_Tp const& value)
        {
            for (uint64_t i = 0; i < m_length; ++i) {
                if (*m_pointers[i] == value) {
                    remove(i);
                    return;
                }
            }
        }

        void remove(_Tp* value) { remove(*value); }

        void removeLast() { remove(m_length - 1); }

        void clear()
        {
            for (uint64_t i = 0; i < m_length; ++i) {
                delete m_pointers[i];
            }
            m_length = 0;
        }

        uint64_t size() const { return m_length; }

        /* --- Operators --- */

        ArrayList& operator=(ArrayList const& other)
        {
            clear();
            m_length    = other.m_length;
            m_capacity  = other.m_capacity;
            m_increment = other.m_increment;
            for (uint64_t i = 0; i < m_length; ++i) {
                m_pointers[i] = other.m_pointers[i];
            }
            return *this;
        }

        ArrayList& operator=(ArrayList&& other) noexcept
        {
            clear();
            m_pointers  = other.m_pointers;
            m_length    = other.m_length;
            m_capacity  = other.m_capacity;
            m_increment = other.m_increment;

            other.m_pointers = nullptr;
            other.m_length   = 0;
            other.m_capacity = 0;
            return *this;
        }

        ArrayList& operator+=(ArrayList const& other)
        {
            for (uint64_t i = 0; i < other.m_length; ++i) {
                add(other.m_pointers[i]);
            }
            return *this;
        }

        ArrayList& operator-=(ArrayList const& other)
        {
            for (uint64_t i = 0; i < other.m_length; ++i) {
                remove(other.m_pointers[i]);
            }
            return *this;
        }

        ArrayList& operator+=(_Tp const& value)
        {
            add(value);
            return *this;
        }

        ArrayList& operator-=(_Tp const& value)
        {
            remove(value);
            return *this;
        }

        _Tp& operator[](uint64_t index) { return *m_pointers[index]; }

    private:
        _Tp**    m_pointers;
        uint64_t m_length;
        uint64_t m_capacity;
        bool     m_increment;
    };
}