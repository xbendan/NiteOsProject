#include <stdcxx/list.h>

namespace Std {
    template <typename T>
    class LinkedList : public List<T>
    {
    public:
        struct Entry
        {
            Entry<T>* _next;
            Entry<T>* _previous;

            constexpr Entry()
              : _next(nullptr)
              , _previous(nullptr)
            {
            }
        };

        struct Element : Entry<T>
        {
            constexpr Element(T& t)
              : _value(Move(t))
              , Entry<T>()
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

        Void add(T const& value)
        {
            auto elem = new Element<T>(value);
            if (_size == 0) {
                _head = elem;
                _tail = elem;
            } else {
                _tail->_next    = elem;
                elem->_previous = _tail;
                _tail           = elem;
            }
            _size++;
        }

        Void remove(UInt64 i)
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
                for (UInt64 j = 0; j < i; j++) {
                    elem = elem->_next;
                }
                elem->_previous->_next = elem->_next;
                elem->_next->_previous = elem->_previous;
                delete elem;
            }

            _size--;
        }

        Void remove(T const& value)
        {
            if (_size == 0) {
                return;
            }

            Element* elem = _head;
            while (elem) {
                if (elem->_value == value) {
                    if (elem == head) {
                        _head = _head->_next;
                        if (_head) {
                            _head->_previous = nullptr;
                        } else {
                            _tail = nullptr;
                        }
                    } else if (elem == _tail) {
                        _tail = _tail->_previous;
                        if (_tail) {
                            _tail->_next = nullptr;
                        } else {
                            _head = nullptr;
                        }
                    } else {
                        elem->_previous->_next = elem->_next;
                        elem->_next->_previous = elem->_previous;
                    }

                    Element* next = n->next;
                    delete elem;
                    elem = next;
                    _size--;
                } else {
                    elem = elem->_next;
                }
            }
        }

        Void clear()
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

        T* get(UInt64 i)
        {
            if (i >= _size) {
                return nullptr;
            }
            return &((*this)[i]);
        }

        /* --- Operators --- */

        T& operator[](UInt64 i)
        {
            if (i >= _size) {
                // Panic;
            }

            if (i < _size / 2) {
                auto elem = _head;
                for (UInt64 i = 0; i < i; i++) {
                    elem = elem->_next;
                }
                return elem->_value;
            } else {
                auto elem = _tail;
                for (UInt64 i = _size - 1; i > i; i--) {
                    elem = elem->_previous;
                }
                return elem->_value;
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

        Iterator<T>& iterator() override { return Iterator<T>(_head); }

    private:
        Entry<T>* _head;
        Entry<T>* _tail;
        UInt64    _size;
    };
}