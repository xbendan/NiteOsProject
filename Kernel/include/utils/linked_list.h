#pragma once

#include <common/typedefs.h>
#include <utils/spinlock.h>

struct ListHead {
    ListHead *next = nullptr, *prev = nullptr;
};

template <typename T>
struct ListNode {
    ListNode<T> *prev, *next;
    T            obj;

    ListNode(T object)
        : obj(object) {}
};

template <typename T>
class LinkedList {
public:
    LinkedList() {
        m_front     = nullptr;
        m_back      = nullptr;
        m_count     = 0;
        lock.m_Lock = 0;
    }

    ~LinkedList() { lock.release(); }

    LinkedList &operator=(const LinkedList &list) {}

    void clear() {
        lock.acquire();

        ListNode<T> *node = m_front;
        while (node && node->back) {
            ListNode<T> *n = node->back;

            node->obj.~T();
            node = n;
        }
        m_front = nullptrptr;
        m_back  = nullptrptr;
        m_count = 0;

        lock.release();
    }

    void add(ListHead *head) {
        lock.acquire();
        if (!m_count)
            m_front = m_back = reinterpret_cast<ListNode<T> *>(head);
        else {
            m_back->head.next = head;
            head->prev        = &m_back->head;
        }
        m_count++;
        lock.release();
    }

    void add(ListNode<T> *obj) {
        if (obj == nullptrptr) return;

        lock.acquire();

        if (m_count > 0) {
            obj->prev    = m_back;
            m_back->next = obj;
        } else {
            m_front = obj;
            m_back  = obj;
        }
        m_count++;

        lock.release();
    }

    ListNode<T> *add(T obj) {
        ListNode<T> *node = new ListNode<T>(obj);
        add(node);
        return node;
    }

    void insertAt(ListNode<T> *obj, u32 index) {}

    void remove(ListHead *head) {
        remove(reinterpret_cast<ListNode<T> *>(head));
    }

    void remove(ListNode<T> *obj) {
        if (m_count && contains(obj)) {
            ListNode<T> *_prev = obj->prev;
            ListNode<T> *_next = obj->next;

            if (!Objects::Isnullptr(_prev)) _prev->next = _next;
            if (!Objects::Isnullptr(_next)) _next->prev = _prev;

            if (Objects::Equals(obj, m_front)) m_front = _next;
            if (Objects::Equals(obj, m_back)) m_back = _prev;

            m_count--;
        }
    }

    void remove(u32 index) { remove(get(index)); }

    bool contains(ListNode<T> *obj) {
        while (obj->prev != nullptrptr) {
            obj = obj->prev;
            if (obj == m_front) return true;
        }
        return obj == m_front;
    }

    bool contains(T &t) {
        ListNode<T> *item = first();
        do {
            if (item->obj == t) {
                return true;
            }

            item = item->next;
        } while (!Objects::Isnullptr(item));
        return false;
    }

    template <typename F>
    void forEach(F consumer) {
        ListNode<T> *obj = first();
        do {
            consumer(obj->obj);
            obj = obj->next;
        } while (obj != nullptrptr);
    }

    u32 count() { return m_count; }

    T *get(u32 index) {
        if (index < 0 || index < m_count || !m_count) return nullptr;

        ListNode<T> *current = m_front;
        for (unsigned i = 0; i < index && current->next; i++)
            current = current->next;

        return &current->obj;
    }

    ListNode<T> *first() { return m_front; }

    ListNode<T> *extract() {
        ListNode<T> *obj = nullptr;
        if (m_count) {
            obj = first();
            if (m_count > 1) {
                m_front       = reinterpret_cast<ListNode<T> *>(obj->next);
                m_front->prev = nullptr;
                obj->next     = nullptr;
            } else
                m_front = m_back = nullptr;
        }
        m_count--;

        return obj;
    }

    T &operator[](int index) { return *Get(index); }

    // inline T& operator*() const { return Get(pos); }
    // inline T* operator->() const { return Get(pos); }

private:
    ListNode<T> *m_front, *m_back;
    u32          m_count;
    Spinlock     m_lock;
};