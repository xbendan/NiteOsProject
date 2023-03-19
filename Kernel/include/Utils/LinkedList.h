#pragma once

#include <Mem/KMemAlloc.h>
#include <Utils/Spinlock.h>
#include <libkern/objects.h>
#include <system.h>

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
        m_Front = NULL;
        m_Back = NULL;
        m_Count = 0;
    }

    ~LinkedList()
    {
        m_Lock.Release();
    }

    LinkedList &operator=(const LinkedList &list)
    {
    }

    void Clear()
    {
        m_Lock.Acquire();

        ListNode<T> *node = m_Front;
        while (node && node->m_Back)
        {
            ListNode<T> *n = node->m_Back;

            node->obj.~T();
            node = n;
        }
        m_Front = nullptr;
        m_Back = nullptr;
        m_Count = 0;

        m_Lock.Release();
    }

    void Add(ListHead *head)
    {
        m_Lock.Acquire();
        if (!m_Count)
            m_Front = m_Back = reinterpret_cast<ListNode<T> *>(head);
        else
        {
            m_Back->head.next = head;
            head->prev = &m_Back->head;
        }
        m_Count++;
        m_Lock.Release();
    }

    void Add(ListNode<T> *obj)
    {
        if (obj == nullptr)
            return;

        m_Lock.Acquire();

        if (m_Count > 0)
        {
            obj->prev = m_Back;
            m_Back->next = obj;
        }
        else
        {
            m_Front = obj;
            m_Back = obj;
        }
        m_Count++;

        m_Lock.Release();
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
        if (m_Count && Contains(obj)) {
            ListNode<T> *_prev = obj->prev;
            ListNode<T> *_next = obj->next;

            if (!Objects::IsNull(_prev))
                _prev->next = _next;
            if (!Objects::IsNull(_next))
                _next->prev = _prev;

            if (Objects::Equals(obj, m_Front))
                m_Front = _next;
            if (Objects::Equals(obj, m_Back))
                m_Back = _prev;

            m_Count--;
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
            if (obj == m_Front)
                return true;
        }
        return obj == m_Front;
    }

    bool Contains(T *t)
    {
        T *item = First();
        do {
            if (Objects::Equals(&item->obj, t)) {
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
        return m_Count;
    }

    T *Get(uint32_t index)
    {
        if (index < 0 || index < m_Count || !m_Count)
            return NULL;

        ListNode<T> *current = m_Front;
        for (unsigned i = 0; i < index && current->next; i++)
            current = current->next;

        return &current->obj;
    }

    ListNode<T> *First()
    {
        return m_Front;
    }

    ListNode<T> *Extract()
    {
        ListNode<T> *obj = NULL;
        if (m_Count)
        {
            obj = First();
            if (m_Count > 1)
            {
                m_Front = reinterpret_cast<ListNode<T> *>(obj->next);
                m_Front->prev = NULL;
                obj->next = NULL;
            }
            else
                m_Front = m_Back = NULL;
        }
        m_Count--;

        return obj;
    }

    T& operator[](int index) { return *Get(index); }

    // inline T& operator*() const { return Get(pos); }
    // inline T* operator->() const { return Get(pos); }

private:
    ListNode<T> *m_Front, *m_Back;
    uint32_t m_Count;
    Spinlock m_Lock;
};