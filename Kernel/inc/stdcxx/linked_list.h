#pragma once

#include <stdcxx/stream.h>
#include <stdcxx/type-traits.h>

namespace Std {
    template <typename T>
    // requires isTrivial<T>::value
    class LinkedList
    {
        using _T = Trivial<T>::type;

    public:
        LinkedList()
          : m_head(nullptr)
          , m_tail(nullptr)
          , m_size(0)
        {
        }
        LinkedList(LinkedList const& other)
          : m_head(nullptr)
          , m_tail(nullptr)
          , m_size(0)
        {
            Node* node = other.m_head;
            while (node != nullptr) {
                add(node->_value);
                node = node->_next;
            }
        }
        LinkedList(LinkedList&& other) noexcept
          : m_head(other.m_head)
          , m_tail(other.m_tail)
          , m_size(other.m_size)
        {
            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
        }
        ~LinkedList() { clear(); }

        void add(_T const& value)
        {
            Node* node = new Node(value);
            if (m_head == nullptr) {
                m_head = node;
                m_tail = node;
            } else {
                m_tail->_next   = node;
                node->_previous = m_tail;

                m_tail = node;
            }
            m_size++;
        }

        void remove(_T const& value)
        {
            Node* node = m_head;
            while (node != nullptr) {
                if (node->_value == value) {
                    if (node->_previous != nullptr) {
                        node->_previous->_next = node->_next;
                    } else {
                        m_head = node->_next;
                    }
                    if (node->_next != nullptr) {
                        node->_next->_previous = node->_previous;
                    } else {
                        m_tail = node->_previous;
                    }
                    delete node;
                    m_size--;
                    return;
                }
                node = node->_next;
            }
        }

        void remove(uint64_t index)
        {
            if (index >= m_size) {
                return;
            }

            if (index == 0) {
                Node* n = m_head;
                m_head  = m_head->_next;
                if (m_head) {
                    m_head->_previous = nullptr;
                } else {
                    m_tail = nullptr;
                }
                delete n;
            } else if (index == m_size - 1) {
                Node* n = m_tail;
                m_tail  = m_tail->_previous;
                if (m_tail) {
                    m_tail->_next = nullptr;
                } else {
                    m_head = nullptr;
                }
                delete n;
            } else {
                Node* n = m_head;
                for (uint64_t i = 0; i < index; i++) {
                    n = n->_next;
                }
                n->_previous->_next = n->_next;
                n->_next->_previous = n->_previous;
                delete n;
            }
        }

        void clear()
        {
            Node* node = m_head;
            while (node != nullptr) {
                Node* next = node->_next;
                delete node;
                node = next;
            }
            m_head = nullptr;
            m_tail = nullptr;
            m_size = 0;
        }

        _T get(uint64_t index)
        {
            if (index >= m_size) {
                return nullptr;
            }

            Node* node = m_head;
            for (uint64_t i = 0; i < index; i++) {
                node = node->_next;
            }
            return node->_value;
        }

        size_t size() const { return m_size; }

        /* --- Operators --- */

        LinkedList& operator=(LinkedList const& other)
        {
            clear();
            Node* node = other.m_head;
            while (node != nullptr) {
                add(node->_value);
                node = node->_next;
            }
            return *this;
        }
        LinkedList& operator=(LinkedList&& other) noexcept
        {
            clear();
            m_head = other.m_head;
            m_tail = other.m_tail;
            m_size = other.m_size;

            other.m_head = nullptr;
            other.m_tail = nullptr;
            other.m_size = 0;
            return *this;
        }
        _T& operator[](uint64_t index)
        {
            if (index >= m_size) {
                // TODO: throw exception
            }

            Node* node = m_head;
            for (uint64_t i = 0; i < index; i++) {
                node = node->_next;
            }
            return node->_value;
        }
        LinkedList& operator+=(LinkedList const& other)
        {
            Node* node = other.m_head;
            while (node != nullptr) {
                add(node->_value);
                node = node->_next;
            }
            return *this;
        }
        LinkedList& operator-=(LinkedList const& other)
        {
            Node* node = other.m_head;
            while (node != nullptr) {
                remove(node->_value);
                node = node->_next;
            }
            return *this;
        }
        LinkedList& operator+=(_T const& value)
        {
            add(value);
            return *this;
        }
        LinkedList& operator-=(_T const& value)
        {
            remove(value);
            return *this;
        }

        /* --- Iterators --- */

        [[nodiscard]] Stream<T>* stream() {}

    private:
        struct Node
        {
            Node(_T const& value)
              : _value(value)
              , _next(nullptr)
              , _previous(nullptr)
            {
            }

            _T    _value;
            Node* _next;
            Node* _previous;
        };
        Node*  m_head;
        Node*  m_tail;
        size_t m_size;
    };
}