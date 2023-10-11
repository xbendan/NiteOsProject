#pragma once

#include <common/typedefs.h>
#include <utils/collection.h>
#include <utils/spinlock.h>

namespace utils {
    template <typename T>
    class LinkedListElement
    {
    public:
        LinkedListElement(T&& obj);
        ~LinkedListElement();

        LinkedList<T>* previous() { return m_previous; }
        LinkedList<T>* next() { return m_next; }
        LinkedList<T>* previous(LinkedList<T>* set)
        {
            LinkedList<T>* _ = m_previous;
            m_previous       = set;
            return _;
        }
        LinkedList<T>* next(LinkedList<T>* set)
        {
            LinkedList<T>* _ = m_next;
            m_next           = set;
            return _;
        }
        bool hasPrev() { return m_previous != nullptr; }
        bool hasNext() { return m_next != nullptr; }
        T&   object() { return m_obj; }

    private:
        LinkedList<T>* m_previous;
        LinkedList<T>* m_next;
        T              m_obj;
    };

    template <typename T>
    class LinkedList : Collection<T>
    {
    public:
        LinkedList()
          : m_head(nullptr)
          , m_tail(nullptr)
          , m_count(0)
        {
        }

        ~LinkedList()
        {
            LinkedListElement<T>*elem = head(), next;
            while (true) {
                next = elem->next();
                delete elem;
                if (next == nullptr) {
                    break;
                } else {
                    elem = next;
                }
            }
        }

        int size() override { return m_count; }

        utils::Optional<T> get(unsigned index) override
        {
            if (index >= m_count) {
                return utils::Optional<T>();
            }
            LinkedListElement<T>* elem = m_head;
            for (int i = 0; i < index; i++) {
                elem = elem->next();
            }
            return utils::Optional<T>(&elem->object());
        }

        int indexOf(T& objRef) override
        {
            int                   i    = 0;
            LinkedListElement<T>* elem = m_head;
            do {
                if (elem->object() == objRef) {
                    return i;
                }
                i++;
                elem = elem->next();
            } while (elem != nullptr);
            return i;
        }

        int lastIndexOf(T& objRef) override
        {
            int                   i = 0, last = -1;
            LinkedListElement<T>* elem = m_head;
            while (i < m_count) {
                if (elem->object() == objRef) {
                    last = i;
                }
                i++;
                elem = elem->next();
            }
            return last;
        }

        void insert(T& objRef, unsigned index) {}

        void add(T& objRef) override
        {
            auto elem = new LinkedListElement<T>(objRef);

            if (m_head == nullptr) {
                m_head = elem;
            } else {
                m_head->next(elem);
            }
            m_tail = elem;
            m_count++;
        }

        bool remove(unsigned index) override
        {
            if (index >= m_count) {
                return false;
            }
            LinkedListElement<T>* elem = m_head;
            for (int i = 0; i < index; i++) {
                elem = elem->next();
            }
            if (elem->hasPrev()) {
                elem->previous()->next(elem->next());
            }
            if (elem->hasNext()) {
                elem->next()->previous(elem->previous());
            }
            if (elem == m_tail) {
                m_tail = elem->previous();
            }
            if (elem == m_head) {
                m_head = elem->next();
            }
            delete elem;
        }

        virtual bool remove(T& objRef) = 0;

        virtual Stream<T>& stream() = 0;

        void forEach(utils::function::Function<void(T)> consumer) override {}

        LinkedListElement<T>* head() { return m_head; }
        LinkedListElement<T>* tail() { return m_tail; }

    private:
        LinkedListElement<T>* m_head;
        LinkedListElement<T>* m_tail;
        unsigned int          m_count;
        Spinlock              m_lock;
    };
}

struct ListHead
{
    ListHead *next = nullptr, *prev = nullptr;
};

template <typename T>
struct ListNode
{
    ListNode<T>*prev, *next;
    T           obj;

    ListNode(T object)
      : obj(object)
    {
    }
};

template <typename T>
class LinkedList
{
public:
    LinkedList()
    {
        m_front       = nullptr;
        m_back        = nullptr;
        m_count       = 0;
        m_lock.m_lock = 0;
    }

    ~LinkedList() { m_lock.release(); }

    LinkedList& operator=(const LinkedList& list)
    {
        m_front = list.m_front;
        m_back  = list.m_back;
        m_count = list.m_count;
        return *this;
    }

    void clear()
    {
        m_lock.acquire();

        ListNode<T>* node = m_front;
        while (node && node->back) {
            ListNode<T>* n = node->back;

            node->obj.~T();
            node = n;
        }
        m_front = nullptr;
        m_back  = nullptr;
        m_count = 0;

        m_lock.release();
    }

    void add(ListHead* head)
    {
        m_lock.acquire();
        if (!m_count)
            m_front = m_back = reinterpret_cast<ListNode<T>*>(head);
        else {
            m_back->head.next = head;
            head->prev        = &m_back->head;
        }
        m_count++;
        m_lock.release();
    }

    void add(ListNode<T>* obj)
    {
        if (obj == nullptr)
            return;

        m_lock.acquire();

        if (m_count > 0) {
            obj->prev    = m_back;
            m_back->next = obj;
        } else {
            m_front = obj;
            m_back  = obj;
        }
        m_count++;

        m_lock.release();
    }

    ListNode<T>* add(T obj)
    {
        ListNode<T>* node = new ListNode<T>(obj);
        add(node);
        return node;
    }

    void insertAt(ListNode<T>* obj, u32 index) {}

    void remove(ListHead* head)
    {
        remove(reinterpret_cast<ListNode<T>*>(head));
    }

    void remove(ListNode<T>* obj)
    {
        if (m_count && contains(obj)) {
            ListNode<T>* _prev = obj->prev;
            ListNode<T>* _next = obj->next;

            if (_prev != nullptr)
                _prev->next = _next;
            if (_next != nullptr)
                _next->prev = _prev;

            if (obj == m_front)
                m_front = _next;
            if (obj == m_back)
                m_back = _prev;

            m_count--;
        }
    }

    void remove(T obj)
    {
        ListNode<T>* item = first();
        do {
            if (item->obj == obj) {
                remove(item);
                return;
            }

            item = item->next;
        } while (item != nullptr);
    }

    void remove(u32 index) { remove(get(index)); }

    bool contains(ListNode<T>* obj)
    {
        while (obj->prev != nullptr) {
            obj = obj->prev;
            if (obj == m_front)
                return true;
        }
        return obj == m_front;
    }

    bool contains(T& t)
    {
        ListNode<T>* item = first();
        do {
            if (item->obj == t) {
                return true;
            }

            item = item->next;
        } while (item != nullptr);
        return false;
    }

    template <typename F>
    void forEach(F consumer)
    {
        ListNode<T>* obj = first();
        do {
            consumer(obj->obj);
            obj = obj->next;
        } while (obj != nullptr);
    }

    u32 count() { return m_count; }

    T& get(u32 index)
    {
        ListNode<T>* obj = first();
        if (index >= m_count)
            return obj->obj;
        for (u32 i = 0; i < index; i++) {
            obj = obj->next;
        }
        return obj->obj;
    }

    ListNode<T>* first() { return m_front; }

    ListNode<T>* extract()
    {
        ListNode<T>* obj = nullptr;
        if (m_count) {
            obj = first();
            if (m_count > 1) {
                m_front       = reinterpret_cast<ListNode<T>*>(obj->next);
                m_front->prev = nullptr;
                obj->next     = nullptr;
            } else
                m_front = m_back = nullptr;
        }
        m_count--;

        return obj;
    }

    T& operator[](int index) { return get(index); }

private:
    ListNode<T>*m_front, *m_back;
    u32         m_count;
    Spinlock    m_lock;
};