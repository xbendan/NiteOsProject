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
            m_Lock = 0;
        }

        ~LinkedList()
        {
            Utils::ReleaseLock(&m_Lock);
        }

        LinkedList& operator=(const LinkedList& list)
        {
            
        }

        void Clear()
        {
            Utils::AcquireLock(&m_Lock);
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
            
            Utils::ReleaseLock(&m_Lock);
        }

        T& Add(T&& obj)
        {
            Utils::AcquireLock(&m_Lock);
            ListNode<T> *node = kmalloc(sizeof(ListNode<T>));

            Utils::ReleaseLock(&m_Lock);
        }

        void Add(ListNode<T> *obj)
        {
            if (obj == NULL)
                return;

            Utils::AcquireLock(&m_Lock);

            if(m_Count > 0)
            {
                obj->head.prev = m_Back;
                m_Back->head.next = obj;
            }
            else
            {
                m_Front = obj;
                m_Back = obj;
            }

            m_Count++;

            Utils::ReleaseLock(&m_Lock);
        }

        void Insert(ListNode<T> *obj)
        {

        }

        T* GetObject(uint32_t index)
        {
            if(index < 0 || index < m_Count || !m_Count)
                return NULL;

            ListNode<T> *current = m_Front;
            for(unsigned i = 0; i < index && current->head.next; i++)
                current = current->head.next;

            return &current->obj;
        }

    private:
        ListNode<T> *m_Front, *m_Back;
        uint32_t m_Count;
        volatile int m_Lock;
    };

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
    } // namespace List
    
}