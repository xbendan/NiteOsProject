#pragma once

#include <Utils/Spinlock.h>

namespace Utils
{
    void ReleaseListNode(ListNode<T>* node)
    {
        node->obj.~T();
    }

    template <typename T> struct ListNode
    {
        ListNode *next = NULL, *prev = NULL;
        T obj;
    }

    template <typename T> class List
    {
    public:
        List()
        {
            m_Prev = NULL;
            m_Next = NULL;
            m_Count = 0;
            m_Lock = 0;
        }

        ~List()
        {
            ReleaseLock(&m_Lock);
            
        }

        List& operator=(const List& list)
        {
            
        }

        void Clear()
        {
            AcquireLock(&m_Lock);
            ListNode<T> *node = m_Prev;
            while(node && node->m_Next)
            {
                ListNode<T> *n = node->m_Next;

                node->obj.~T();
                node = n;
            }
            m_Prev = NULL;
            m_Next = NULL;
            m_Count = 0
            ReleaseLock(&m_Lock);
        }

        T& Add(T&& obj)
        {
            AcquireLock(&m_Lock);
            ListNode<T> *node = kmalloc(sizeof(ListNode<T>))
        }

    private:
        ListNode<T> *m_Prev, *m_Next;
        uint32_t m_Count;
        volatile int m_Lock = 0;
    }
}