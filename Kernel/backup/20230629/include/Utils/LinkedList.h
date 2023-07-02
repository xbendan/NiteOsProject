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
        front = NULL;
        back = NULL;
        count = 0;
        lock.lock = 0;
    }

    ~LinkedList()
    {
        lock.Release();
    }

    LinkedList &operator=(const LinkedList &list)
    {
    }

    void Clear()
    {
        lock.Acquire();

        ListNode<T> *node = front;
        while (node && node->back)
        {
            ListNode<T> *n = node->back;

            node->obj.~T();
            node = n;
        }
        front = nullptr;
        back = nullptr;
        count = 0;

        lock.Release();
    }

    void Add(ListHead *head)
    {
        lock.Acquire();
        if (!count)
            front = back = reinterpret_cast<ListNode<T> *>(head);
        else
        {
            back->head.next = head;
            head->prev = &back->head;
        }
        count++;
        lock.Release();
    }

    void Add(ListNode<T> *obj)
    {
        if (obj == nullptr)
            return;

        lock.Acquire();

        if (count > 0)
        {
            obj->prev = back;
            back->next = obj;
        }
        else
        {
            front = obj;
            back = obj;
        }
        count++;

        lock.Release();
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
        if (count && Contains(obj)) {
            ListNode<T> *_prev = obj->prev;
            ListNode<T> *_next = obj->next;

            if (!Objects::IsNull(_prev))
                _prev->next = _next;
            if (!Objects::IsNull(_next))
                _next->prev = _prev;

            if (Objects::Equals(obj, front))
                front = _next;
            if (Objects::Equals(obj, back))
                back = _prev;

            count--;
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
            if (obj == front)
                return true;
        }
        return obj == front;
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
        return count;
    }

    T *Get(uint32_t index)
    {
        if (index < 0 || index < count || !count)
            return NULL;

        ListNode<T> *current = front;
        for (unsigned i = 0; i < index && current->next; i++)
            current = current->next;

        return &current->obj;
    }

    ListNode<T> *First()
    {
        return front;
    }

    ListNode<T> *Extract()
    {
        ListNode<T> *obj = NULL;
        if (count)
        {
            obj = First();
            if (count > 1)
            {
                front = reinterpret_cast<ListNode<T> *>(obj->next);
                front->prev = NULL;
                obj->next = NULL;
            }
            else
                front = back = NULL;
        }
        count--;

        return obj;
    }

    T& operator[](int index) { return *Get(index); }

    // inline T& operator*() const { return Get(pos); }
    // inline T* operator->() const { return Get(pos); }

private:
    ListNode<T> *front, *back;
    uint32_t count;
    Spinlock lock;
};