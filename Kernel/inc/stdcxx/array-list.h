#pragma once

#include <stdcxx/list.h>
#include <stdcxx/type-traits.h>

#define DEFAULT_ARRAY_LIST_CAPACITY 8
#define DEFAULT_ARRAY_LIST_GROWTH_FACTOR 2

namespace Std {
    template <typename T>
    class ArrayList
    {
    public:
        /* --- Constructors --- */
        ArrayList()
          : ArrayList(DEFAULT_ARRAY_LIST_CAPACITY)
        {
        }
        ArrayList(uint32_t capacity,
                  bool     doIncrement = true,
                  float    factor      = DEFAULT_ARRAY_LIST_GROWTH_FACTOR)
          : _buffer(new T[capacity])
          , _capacity(capacity)
          , _length(0)
          , _factor(factor)
          , _increment(doIncrement)
        {
        }
        ArrayList(ArrayList<T> const& other)
          : _buffer(other._buffer)
          , _capacity(other._capacity)
          , _length(other._length)
          , _factor(other._factor)
          , _increment(other._increment)
        {
        }
        ArrayList(ArrayList<T>&& other)
          : _buffer(other._buffer)
          , _capacity(other._capacity)
          , _length(other._length)
          , _factor(other._factor)
          , _increment(other._increment)
        {
            other._buffer   = nullptr;
            other._capacity = 0;
            other._length   = 0;
        }
        template <typename... E>
            requires(IsSame<T, E> && ...)
        ArrayList(E... initial)
        {
            for (auto& item : { initial... }) {
                add(item);
            }
        }
        ~ArrayList()
        {
            delete[] _buffer;
            _length = 0;
        }

        /* --- Methods --- */

        void add(T const& value) { _buffer[_length++] = Std::Move(value); }

        void remove(uint64_t i)
        {
            if (i >= _length) {
                return;
            }
        }

        virtual void remove(T const& t) {}
        virtual void clear() {}
        virtual T*   get(uint64_t i) {}

        /* --- Operators --- */
        T& operator[](uint64_t index)
        {
            if (index >= _length) {
                // TODO: Throw exception
            }
            return _buffer[index];
        }

        ArrayList<T>& operator=(ArrayList<T> const& other)
        {
            if (this == &other) {
                return *this;
            }

            _buffer    = other._buffer;
            _capacity  = other._capacity;
            _length    = other._length;
            _factor    = other._factor;
            _increment = other._increment;

            return *this;
        }
        ArrayList<T>& operator=(ArrayList<T>&& other)
        {
            if (this == &other) {
                return *this;
            }

            _buffer    = other._buffer;
            _capacity  = other._capacity;
            _length    = other._length;
            _factor    = other._factor;
            _increment = other._increment;

            other._buffer   = nullptr;
            other._capacity = 0;
            other._length   = 0;

            return *this;
        }

    private:
        T*       _buffer;
        uint64_t _capacity;
        uint64_t _length;
        float    _factor;
        bool     _increment;
    };
}