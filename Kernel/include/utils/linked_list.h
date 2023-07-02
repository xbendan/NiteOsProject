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
        lock.m_Lock = 0;
    }

    ~LinkedList()
    {
        lock.release();
    }

    LinkedList &operator=(const LinkedList &list)
    {
    }

    void clear()
    {
        lock.acquire();

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

        lock.release();
    }

    void add(ListHead *head)
    {
        lock.acquire();
        if (!count)
            front = back = reinterpret_cast<ListNode<T> *>(head);
        else
        {
            back->head.next = head;
            head->prev = &back->head;
        }
        count++;
        lock.release();
    }

    void add(ListNode<T> *obj)
    {
        if (obj == nullptr)
            return;

        lock.acquire();

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

        lock.release();
    }

    ListNode<T> *add(T obj)
    {
        ListNode<T> *node = new ListNode<T>(obj);
        add(node);
        return node;
    }

    void insertAt(ListNode<T> *obj, uint32_t index)
    {
    }

    void remove(ListHead *head)
    {
        remove(reinterpret_cast<ListNode<T> *>(head));
    }

    void remove(ListNode<T> *obj)
    {
        if (count && contains(obj)) {
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

    void remove(uint32_t index)
    {
        remove(get(index));
    }

    bool contains(ListNode<T> *obj)
    {
        while (obj->prev != nullptr)
        {
            obj = obj->prev;
            if (obj == front)
                return true;
        }
        return obj == front;
    }

    bool contains(T &t)
    {
        ListNode<T> *item = first();
        do {
            if (item->obj == t) {
                return true;
            }

            item = item->next;
        }
        while (!Objects::IsNull(item));
        return false;
    }

    void forEach()
    {
        
    }

    uint32_t count()
    {
        return count;
    }

    T *get(uint32_t index)
    {
        if (index < 0 || index < count || !count)
            return NULL;

        ListNode<T> *current = front;
        for (unsigned i = 0; i < index && current->next; i++)
            current = current->next;

        return &current->obj;
    }

    ListNode<T> *first()
    {
        return front;
    }

    ListNode<T> *extract()
    {
        ListNode<T> *obj = NULL;
        if (count)
        {
            obj = first();
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