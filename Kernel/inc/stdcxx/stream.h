#pragma once

#include <stdcxx/func.h>
#include <stdcxx/opt.h>
#include <stdcxx/refptr.h>

namespace Std {
    template <typename T>
    class LinkedList;

    template <typename T>
    class Array;

    template <typename T>
    class Stream
    {
    public:
        using _T = Trivial<T>::type;

        Stream(uint64_t size)
          : m_size(size)
          , m_end(size)
          , m_offset(0)
          , m_buffer(new _T[size])
        {
        }
        Stream(Stream const& other)
          : m_size(new _T[other.length()])
          , m_size(other.length())
          , m_end(other.length())
          , m_offset(0)
        {
            for (int i = 0; i < m_size; i++) {
                m_buffer[i] = other.m_buffer[i];
            }
        }
        Stream(Stream&& other)
          : m_size(other.m_size)
          , m_end(other.m_end)
          , m_offset(other.m_offset)
          , m_buffer(other.m_buffer)
        {
            other.m_buffer = nullptr;
            other.m_size   = 0;
            other.m_end    = 0;
            other.m_offset = 0;
        }
        ~Stream() { delete m_buffer; }

        Stream<T>& filter(Predicate<_T> predicate)
        {
            _T  buf[m_size];
            int p = 0;
            for (int i = m_offset; i < m_size; i++) {
                if (predicate(m_buffer[i])) {
                    buf[p++] = m_buffer[i];
                }
            }
            for (int i = 0; i < p; i++) {
                m_buffer[i] = buf[i];
            }
            m_end    = p;
            m_offset = 0;
            return *this;
        }

        template <typename R>
        Stream<typename Trivial<R>::type>& mapTo(
          Function<typename Trivial<R>::type(_T)> func)
        {
            Stream<typename Trivial<R>::type>* buf =
              new Stream<typename Trivial<R>::type>(m_size);
            for (int i = m_offset; i < m_end; i++) {
                buf->m_buffer[i] = func(m_buffer[i]);
            }
            return *buf;
        }

        template <typename R>
        Stream<R>& flatMapTo(Function<R(_T)> func)
        {

            return *this;
        }

        Stream<T>& forEach(Consumer<T&> consumer)
        {
            for (int i = m_offset; i < m_end; i++) {
                consumer(m_buffer[i]);
            }
            return *this;
        }

        Stream<T>& forEachIndexed(Function<void(_T, uint64_t)> func)
        {
            for (int i = m_offset; i < m_end; i++) {
                func(m_buffer[i], i);
            }
            return *this;
        }

        Stream<T>& limit(uint64_t limit)
        {
            m_size = length() < limit ? length() : limit;
            return *this;
        }

        bool allMatch(Predicate<_T> predicate)
        {
            for (int i = m_offset; i < m_size; i++) {
                if (!predicate(m_buffer[i])) {
                    return false;
                }
            }
            return true;
        }

        bool anyMatch(Predicate<_T> predicate)
        {
            for (int i = m_offset; i < m_size; i++) {
                if (predicate(m_buffer[i])) {
                    return true;
                }
            }
            return false;
        }

        bool noneMatch(Predicate<_T> predicate)
        {
            for (int i = m_offset; i < m_size; i++) {
                if (predicate(m_buffer[i])) {
                    return false;
                }
            }
            return true;
        }

        Opt<T> findFirst()
        {
            if (length())
                return Opt<T>(m_buffer[m_offset++]);
            else
                return Opt<T>::empty();
        }

        Opt<T> findLast()
        {
            if (m_end > m_offset) {
                _T val = m_buffer[m_end - 1];
                m_end--;
                return val;
            }
            return Opt<_T>::empty();
        }

        Stream<T>& skip(uint64_t amount)
        {
            m_offset += amount;
            return *this;
        }

        Stream<T>& truncate()
        {
            if (m_offset || m_end != m_size) {
                _T* buf = new _T[length()];
                for (int i = m_offset; i < m_end; i++) {
                    buf[i] = m_buffer[i];
                }
            }
            return *this;
        }

        size_t length() { return m_end - m_offset; }

    private:
        friend class LinkedList<T>;
        friend class Array<T>;

        _T*    m_buffer;
        size_t m_size;
        size_t m_end;
        size_t m_offset;
    };
}