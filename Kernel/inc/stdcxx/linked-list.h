#pragma once

#include <stdcxx/copy.h>
#include <stdcxx/list.h>
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

            bool isLinked() { return _next || _previous; }
        };

        struct Element : Entry
        {
            constexpr Element(T const& t)
              : Entry()
              , _value(Std::Move(t))
            {
            }
            T _value;
        };

        /* --- Constructors --- */

        LinkedList()
          : _head(nullptr)
          , _tail(nullptr)
          , _size(0)
        {
        }

        LinkedList(LinkedList<T> const& other)
        {
            if (!other._size) {
                return;
            }
            _head = other._head;
            _tail = other._tail;
            _size = other._size;
        }
        ~LinkedList() = default;

        /* --- Methods --- */

        void add(T const& value)
        {
            Entry* elem = (Std::IsBaseOf<Entry, T>::Value) ? (Entry*)&value
                                                           : new Element(value);
            if (_size == 0) {
                _head = elem;
                _tail = elem;
            } else {
                _tail->_next    = elem;
                elem->_previous = _tail;

                _tail = elem;
            }
            _size++;
        }

        template <typename _T>
            requires Std::IsBaseOf<Entry, _T>::Value
        void add(T* value)
        {
            Entry* elem = value;
            if (_size == 0) {
                _head = elem;
                _tail = elem;
            } else {
                _tail->_next    = elem;
                elem->_previous = _tail;

                _tail = elem;
            }
            _size++;
        }

        void remove(uint64_t i)
        {
            if (i >= _size) {
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
            } else if (i == _size - 1) {
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

            _size--;
        }

        void remove(T const& value, bool del)
        {
            if (_size == 0) {
                return;
            }

            Entry* elem = _head;

            while (elem) {
                if (_size == 1) {
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
                _size--;
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
            _head = nullptr;
            _tail = nullptr;
            _size = 0;
        }

        T* get(uint64_t i)
        {
            if (i >= _size) {
                return nullptr;
            }
            Entry* elem = _head;
            for (uint64_t j = 0; j < i; j++) {
                elem = elem->_next;
            }
            return Std::IsBaseOf<Entry, T>::Value ? (T*)elem
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
            _size--;
            return Std::IsBaseOf<Entry, T>::Value ? (T*)elem
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
            _size--;
            return Std::IsBaseOf<Entry, T>::Value ? (T*)ent
                                                  : &((Element*)ent)->_value;
        }

        size_t size() { return _size; }

        /* --- Operators --- */

        T& operator[](uint64_t _index)
        {
            if (_index >= _size) {
                // Panic;
            }

            if (_index < _size / 2) {
                Entry* elem = _head;
                for (uint64_t i = 0; i < _index; i++) {
                    elem = elem->_next;
                }
                return (Std::IsBaseOf<Entry, T>::Value
                          ? (T&)*elem
                          : ((Element*)elem)->_value);
            } else {
                Entry* elem = _tail;
                for (uint64_t i = _size - 1; i > _index; i--) {
                    elem = elem->_previous;
                }
                return (Std::IsBaseOf<Entry, T>::Value
                          ? (T&)*elem
                          : ((Element*)elem)->_value);
            }
        }

        LinkedList& operator=(LinkedList<T> const& other)
        {
            if (this != &other) {
                if (!other._size) {
                    return *this;
                }
                _head = other._head;
                _tail = other._tail;
                _size = other._size;
            }
            return *this;
        }

    private:
        Entry*   _head;
        Entry*   _tail;
        uint64_t _size;
    };
}