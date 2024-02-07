#pragma once

#include <stdcxx/list.h>
#include <stdcxx/move.h>
#include <stdcxx/stream.h>
#include <stdcxx/type-traits.h>

namespace Std {
    template <typename T>
    class LinkedList
    {
    public:
        struct Entry
        {
            Entry* _next;
            Entry* _previous;

            constexpr Entry()
              : _next(nullptr)
              , _previous(nullptr)
            {
            }
        };

        struct Element : Entry
        {
            constexpr Element(T const& t)
              : Entry()
              , _value(Std::move(t))
            {
            }
            T _value;
        };

        /* --- Constructors --- */

        LinkedList()
          : _head(nullptr)
          , _tail(nullptr)
          , m_size(0)
        {
        }

        LinkedList(LinkedList<T> const& other)
        {
            if (!other.m_size) {
                return;
            }
            _head  = other._head;
            _tail  = other._tail;
            m_size = other.m_size;
        }

        LinkedList(LinkedList<T>&& other)
        {
            if (!other.m_size) {
                return;
            }
            _head        = other._head;
            _tail        = other._tail;
            m_size       = other.m_size;
            other._head  = nullptr;
            other._tail  = nullptr;
            other.m_size = 0;
        }

        ~LinkedList() = default;

        /* --- Methods --- */

        void add(T const& value)
        {
            Entry* elem = (Std::isBaseOf<Entry, T>::Value) ? (Entry*)&value
                                                           : new Element(value);
            if (m_size == 0) {
                _head = elem;
                _tail = elem;
            } else {
                _tail->_next    = elem;
                elem->_previous = _tail;

                _tail = elem;
            }
            m_size++;
        }

        template <typename _T>
            requires Std::isBaseOf<Entry, _T>::Value
        void add(T* value)
        {
            Entry* elem = value;
            if (m_size == 0) {
                _head = elem;
                _tail = elem;
            } else {
                _tail->_next    = elem;
                elem->_previous = _tail;

                _tail = elem;
            }
            m_size++;
        }

        void remove(uint64_t i)
        {
            if (i >= m_size) {
                return;
            }

            if (i == 0) {
                auto elem = _head;
                _head     = _head->_next;
                if (_head) {
                    _head->_previous = nullptr;
                } else {
                    _tail = nullptr;
                }
                delete elem;
            } else if (i == m_size - 1) {
                auto elem = _tail;
                _tail     = _tail->_previous;
                if (_tail) {
                    _tail->_next = nullptr;
                } else {
                    _head = nullptr;
                }
                delete elem;
            } else {
                auto elem = _head;
                for (uint64_t j = 0; j < i; j++) {
                    elem = elem->_next;
                }
                elem->_previous->_next = elem->_next;
                elem->_next->_previous = elem->_previous;
                delete elem;
            }

            m_size--;
        }

        void remove(T const& value, bool del)
        {
            if (m_size == 0) {
                return;
            }

            Entry* elem = _head;

            while (elem) {
                if (m_size == 1) {
                    _head = nullptr;
                    _tail = nullptr;
                } else if (elem == _head) {
                    _head            = _head->_next;
                    _head->_previous = nullptr;
                } else if (elem == _tail) {
                    _tail        = _tail->_previous;
                    _tail->_next = nullptr;
                } else {
                    elem->_previous->_next = elem->_next;
                    elem->_next->_previous = elem->_previous;
                }
                Entry* n = elem->_next;
                if (del) {
                    delete elem;
                }
                elem = n;
                m_size--;
            }
        }

        void remove(T const& value) { remove(value, true); }

        void clear()
        {
            auto elem = _head;
            while (elem) {
                auto next = elem->_next;
                delete elem;
                elem = next;
            }
            _head  = nullptr;
            _tail  = nullptr;
            m_size = 0;
        }

        T* get(uint64_t i)
        {
            if (i >= m_size) {
                return nullptr;
            }
            Entry* elem = _head;
            for (uint64_t j = 0; j < i; j++) {
                elem = elem->_next;
            }
            return Std::isBaseOf<Entry, T>::Value ? (T*)elem
                                                  : &((Element*)elem)->_value;
        }

        T* takeFirst()
        {
            Entry* elem = _head;
            if (_head == _tail) {
                _head = nullptr;
                _tail = nullptr;
            } else {
                _head            = _head->_next;
                _head->_previous = nullptr;
            }
            m_size--;
            return Std::isBaseOf<Entry, T>::Value ? (T*)elem
                                                  : &((Element*)elem)->_value;
        }

        T* takeLast()
        {
            Entry* ent = _tail;
            if (_head == _tail) {
                _head = nullptr;
                _tail = nullptr;
            } else {
                _tail          = _tail->_previous;
                _tail->_next   = nullptr;
                ent->_previous = nullptr;
            }
            m_size--;
            return Std::isBaseOf<Entry, T>::Value ? (T*)ent
                                                  : &((Element*)ent)->_value;
        }

        size_t size() { return m_size; }

        [[nodiscard]] Stream<T>* stream()
        {
            Stream<T>* stream = new Stream<T>(m_size);
            Entry*     ent    = _head;
            for (int i = 0; i < m_size; i++) {
                stream->m_buffer[i] = WeakRef<T>(Std::isBaseOf<Entry, T>::Value
                                                   ? *(T*)ent
                                                   : ((Element*)ent)->_value);
                ent                 = ent->_next;
            }
            return stream;
        }

        /* --- Operators --- */

        T& operator[](uint64_t _index)
        {
            if (_index >= m_size) {
                // Panic;
            }

            if (_index < m_size / 2) {
                Entry* elem = _head;
                for (uint64_t i = 0; i < _index; i++) {
                    elem = elem->_next;
                }
                return (Std::isBaseOf<Entry, T>::Value
                          ? (T&)*elem
                          : ((Element*)elem)->_value);
            } else {
                Entry* elem = _tail;
                for (uint64_t i = m_size - 1; i > _index; i--) {
                    elem = elem->_previous;
                }
                return (Std::isBaseOf<Entry, T>::Value
                          ? (T&)*elem
                          : ((Element*)elem)->_value);
            }
        }

        LinkedList& operator=(LinkedList<T> const& other)
        {
            if (this != &other) {
                if (!other.m_size) {
                    return *this;
                }
                _head  = other._head;
                _tail  = other._tail;
                m_size = other.m_size;
            }
            return *this;
        }

    private:
        Entry*   _head;
        Entry*   _tail;
        uint64_t m_size;
    };
}