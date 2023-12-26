#include <stdcxx/list.h>

namespace Std {
    template <typename T>
    class LinkedList : public List<T>
    {
    public:
        using IsObjectInherited = Std::IsBaseOf<Entry<T>, T>;

        struct Entry
        {
            using EntryType = Entry<T>;

            EntryType* _next;
            EntryType* _previous;

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

        void add(T const& value)
        {
            Entry<T>* elem = IsObjectInherited ? &value : new Element<T>(value);
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

        void remove(T const& value)
        {
            if (_size == 0) {
                return;
            }

            auto* elem = _head;
            while (elem) {
                if ((IsObjectInherited ? *elem : elem->_value) == value) {
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

                    auto* next = n->next;
                    delete elem;
                    elem = next;
                    _size--;
                } else {
                    elem = elem->_next;
                }
            }
        }

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
            auto elem = _head;
            for (uint64_t j = 0; j < i; j++) {
                elem = elem->_next;
            }
            return Std::IsBaseOf<Entry<T>, T> ? elem : &elem->_value;
        }

        /* --- Operators --- */

        T& operator[](uint64_t i)
        {
            if (i >= _size) {
                // Panic;
            }

            if (i < _size / 2) {
                auto elem = _head;
                for (uint64_t i = 0; i < i; i++) {
                    elem = elem->_next;
                }
                return (Std::IsBaseOf<Entry<T>, T>() ? *elem : elem->_value)
            } else {
                auto elem = _tail;
                for (uint64_t i = _size - 1; i > i; i--) {
                    elem = elem->_previous;
                }
                return (Std::IsBaseOf<Entry<T>, T>() ? *elem : elem->_value)
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
        Element* _head;
        Element* _tail;
        uint64_t _size;
    };
}