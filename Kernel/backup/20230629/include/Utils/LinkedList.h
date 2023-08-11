#pragma once

#include <Utils/Spinlock.h>
#include <libkern/objects.h>
#include <System.h>

typedef struct ListHead
{
    ListHead *next = NULL, *prev = NULL;
} listhead_t;

template <typename T>
struct ListNode
{
    ListNode<T> *prev, *next;
    T obj;

    ListNode(T object)
      : obj(object) { }
};

template <typename T>
class LinkedList
{
public:
    LinkedList()
    {
        m_front = NULL;
        m_back = NULL;
        m_count = 0;
        m_lock.lock = 0;
    }

    ~LinkedList()
    {
        m_lock.Release();
    }

    LinkedList &operator=(const LinkedList &list)
    {
    }

    void Clear()
    {
        m_lock.Acquire();

        ListNode<T> *node = m_front;
        while (node && node->back)
        {
            ListNode<T> *n = node->back;

            node->obj.~T();
            node = n;
        }
        m_front = nullptr;
        m_back = nullptr;
        m_count = 0;

        m_lock.Release();
    }

    void Add(ListHead *head)
    {
        m_lock.Acquire();
        if (!m_count)
            m_front = m_back = reinterpret_cast<ListNode<T> *>(head);
        else
        {
            m_back->head.next = head;
            head->prev = &m_back->head;
        }
        m_count++;
        m_lock.Release();
    }

    void Add(ListNode<T> *obj)
    {
        if (obj == nullptr)
            return;

        m_lock.Acquire();

        if (m_count > 0)
        {
            obj->prev = m_back;
            m_back->next = obj;
        }
        else
        {
            m_front = obj;
            m_back = obj;
        }
        m_count++;

        m_lock.Release();
    }

    ListNode<T> *Add(T obj)
    {
        ListNode<T> *node = new ListNode<T>(obj);
        Add(node);
        return node;
    }

    void InsertAt(ListNode<T> *obj, uint32_t index)
    {
    }

    void Remove(ListHead *head)
    {
        Remove(reinterpret_cast<ListNode<T> *>(head));
    }

    void Remove(ListNode<T> *obj)
    {
        if (m_count && Contains(obj)) {
            ListNode<T> *_prev = obj->prev;
            ListNode<T> *_next = obj->next;

            if (!Objects::IsNull(_prev))
                _prev->next = _next;
            if (!Objects::IsNull(_next))
                _next->prev = _prev;

            if (Objects::Equals(obj, m_front))
                m_front = _next;
            if (Objects::Equals(obj, m_back))
                m_back = _prev;

            m_count--;
        }
    }

    void Remove(uint32_t index)
    {
        Remove(Get(index));
    }

    bool Contains(ListNode<T> *obj)
    {
        while (obj->prev != nullptr)
        {
            obj = obj->prev;
            if (obj == m_front)
                return true;
        }
        return obj == m_front;
    }

    bool Contains(T &t)
    {
        ListNode<T> *item = First();
        do {
            if (item->obj == t) {
                return true;
            }

            item = item->next;
        }
        while (!Objects::IsNull(item));
        return false;
    }

    void ForEach()
    {
        
    }

    uint32_t Count()
    {
        return m_count;
    }

    T *Get(uint32_t index)
    {
        if (index < 0 || index < m_count || !m_count)
            return NULL;

        ListNode<T> *current = m_front;
        for (unsigned i = 0; i < index && current->next; i++)
            current = current->next;

        return &current->obj;
    }

    ListNode<T> *First()
    {
        return m_front;
    }

    ListNode<T> *Extract()
    {
        ListNode<T> *obj = NULL;
        if (m_count)
        {
            obj = First();
            if (m_count > 1)
            {
                m_front = reinterpret_cast<ListNode<T> *>(obj->next);
                m_front->prev = NULL;
                obj->next = NULL;
            }
            else
                m_front = m_back = NULL;
        }
        m_count--;

        return obj;
    }

    T& operator[](int index) { return *Get(index); }

    // inline T& operator*() const { return Get(pos); }
    // inline T* operator->() const { return Get(pos); }

private:
    ListNode<T> *m_front, *m_back;
    uint32_t m_count;
    Spinlock m_lock;
};