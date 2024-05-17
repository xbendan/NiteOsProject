#pragma once

#include <mixins/std/type_traits.h>
#include <mixins/types.h>

namespace mixins::utils {
    template <typename _Tp>
        requires(!std::isTrivial<_Tp>::value)
    class LinkedList
    {
        using T = _Tp*;

    public:
        /* --- Constructors --- */

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

        /* --- Methods --- */

        _Tp* pushBack(_Tp const& value) { return pushBack(&value); }

        _Tp* pushBack(_Tp* value)
        {
            Node* n = new Node(value);
            if (m_head == nullptr) {
                m_head = n;
                m_tail = n;
            } else {
                m_tail->_next = n;
                n->_previous  = m_tail;

                m_tail = n;
            }
            m_size++;
            return value;
        }

        _Tp* pushFront(_Tp const& value) { return pushFront(&value); }

        _Tp* pushFront(_Tp* value)
        {
            Node* n = new Node(value);
            if (m_head == nullptr) {
                m_head = n;
                m_tail = n;
            } else {
                m_head->_previous = n;
                n->_next          = m_head;

                m_head = n;
            }
            m_size++;
            return value;
        }

        _Tp* popBack() {}

        _Tp* popFront() {}

        int indexOf(_Tp const& value) {}

        int lastIndexOf(_Tp const& value) {}

        size_t size() const { return m_size; }

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

        _Tp& operator[](uint64_t index) {}

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

        LinkedList& operator+=(_Tp const& value)
        {
            add(value);
            return *this;
        }

        LinkedList& operator-=(_Tp const& value)
        {
            remove(value);
            return *this;
        }

    private:
        struct Node
        {
            Node(T value)
              : _value(value)
              , _next(nullptr)
              , _previous(nullptr)
            {
            }

            T     _value;
            Node* _next;
            Node* _previous;
        };

        Node*  m_head;
        Node*  m_tail;
        size_t m_size;
    };

}