#pragma once

#include <Memory/KMalloc.h>
#include <Utils/Spinlock.h>

namespace Utils
{
    typedef struct ListHead
    {
        ListHead *next = NULL, *prev = NULL;
    } listhead_t;

    template <typename T> struct ListNode
    {
        struct ListHead head;
        T obj;
    };

    template <typename T> class LinkedList
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

        LinkedList& operator=(const LinkedList& list)
        {
            
        }

        void Clear()
        {
            m_Lock.Acquire();
            
            ListNode<T> *node = m_Front;
            while(node && node->m_Back)
            {
                ListNode<T> *n = node->m_Back;

                node->obj.~T();
                node = n;
            }
            m_Front = NULL;
            m_Back = NULL;
            m_Count = 0;
            
            m_Lock.Release();
        }

        void Add(ListHead *head)
        {
            m_Lock.Acquire();
            if (!m_Count)
                m_Front = m_Back = reinterpret_cast<ListNode<T> *>(head);
            else {
                m_Back->head.next = head;
                head->prev = &m_Back->head;
            }
            m_Count++;
            m_Lock.Release();
        }

        void Add(ListNode<T> *obj)
        {
            if (obj == NULL)
                return;

            m_Lock.Acquire();

            if(m_Count > 0)
            {
                obj->head.prev = &m_Back->head;
                m_Back->head.next = &obj->head;
            }
            else
            {
                m_Front = obj;
                m_Back = obj;
            }
            m_Count++;

            m_Lock.Release();
        }

        void InsertAt(ListNode<T> *obj, uint32_t index)
        {

        }

        void Remove(ListHead *head)
        {
            ListHead *prev = head->prev, *next = head->next;

            if(prev != NULL) prev->next = next;
            if(next != NULL) next->prev = prev;

            if(head == &m_Front->head)
            {
                m_Front = (ListNode<T> *) next;
            }
            if(head == &m_Back->head)
            {
                m_Back = (ListNode<T> *) prev;
            }
        }

        void Remove(ListNode<T> *obj) {
            if(m_Count && Contains(obj))
                Remove(&obj->head);
        }

        void Remove(uint32_t index)
        {

        }

        bool Contains(ListNode<T> *obj)
        {
            while(obj->head.prev != NULL)
            {
                obj = (ListNode<T> *) obj->head.prev;
                if(obj == m_Front)
                    return true;
            }
            return obj == m_Front;
        }

        bool Contains(T *t)
        {

        }

        uint32_t Count()
        {
            return m_Count;
        }

        T* Get(uint32_t index)
        {
            if(index < 0 || index < m_Count || !m_Count)
                return NULL;

            ListNode<T> *current = m_Front;
            for(unsigned i = 0; i < index && current->head.next; i++)
                current = current->head.next;

            return &current->obj;
        }

        ListNode<T> *First()
        {
            return m_Front;
        }

        ListNode<T> *Extract()
        {
            ListNode<T> *obj = NULL;
            if (m_Count) {
                obj = First();
                if (m_Count > 1) {
                    m_Front = reinterpret_cast<ListNode<T> *>(obj->head.next);
                    m_Front->head.prev = NULL;
                    obj->head.next = NULL;
                }
                else m_Front = m_Back = NULL;
            }
            return obj;
        }

    private:
        ListNode<T> *m_Front, *m_Back;
        uint32_t m_Count;
        Utils::Spinlock m_Lock;
    };

    /*
    namespace List
    {
        void Remove(listhead_t *head)
        {
            listhead_t *prev = head->prev, *next = head->next;

            if(prev != NULL) prev->next = next;
            if(next != NULL) next->prev = prev;
        }

        void Append(listhead_t* current, listhead_t* node)
        {
            if(current == NULL || node == NULL)
                return;
            else
            {
                node->prev = current;
                if(current->next != NULL)
                {
                    node->next = current->next;
                    current->next->prev = node;
                }
                current->next = node;
            }
        }
    }*/ // namespace List
    
}