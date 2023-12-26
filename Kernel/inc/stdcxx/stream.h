#include <stdcxx/array.h>
#include <stdcxx/func.h>

namespace Std {
    template <typename T>
    class Stream
    {
    public:
        Stream& filter(Function<bool(T&)> func)
        {
            Array<T&>* buf = new Array<T&>(m_size);
            uint64_t   i   = 0;
            for (T& t : *m_buffer) {
                if (func(t)) {
                    (*buf)[i++] = t;
                }
            }
            return *this;
        }

        template <typename R>
        Stream<R>& mapTo(Function<R(T&)> func)
        {
            Stream<R>* buf = new Stream<R>(m_size);
            uint64_t   i   = 0;
            for (T& t : *m_buffer) {
                (*buf)[i++] = func(t);
            }
            return *buf;
        }

        Stream<T>& forEach(Function<void(T&)> func)
        {
            for (T& t : *m_buffer) {
                func(t);
            }
            return *this;
        }

        Stream<T>& forEachIndexed(Function<void(T&, uint64_t)> func)
        {
            uint64_t i = 0;
            for (T& t : *m_buffer) {
                func(t, i++);
            }
            return *this;
        }

        Stream<T>& limit(uint64_t limit)
        {
            m_size = limit;
            return *this;
        }

        bool allMatch(Function<bool(T&)> func)
        {
            for (T& t : *m_buffer) {
                if (!func(t)) {
                    return false;
                }
            }
            return true;
        }

        bool anyMatch(Function<bool(T&)> func)
        {
            for (T& t : *m_buffer) {
                if (func(t)) {
                    return true;
                }
            }
            return false;
        }

        bool noneMatch(Function<bool(T&)> func)
        {
            for (T& t : *m_buffer) {
                if (func(t)) {
                    return false;
                }
            }
            return true;
        }

        Stream<T>& skip(uint64_t amount)
        {
            m_position += amount;
            return *this;
        }

        T& operator[](uint64_t index) { return (*m_buffer)[index]; }

    private:
        Array<T&>* m_buffer;
        size_t     m_size;
        size_t     m_position;
    };
}