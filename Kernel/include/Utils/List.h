#pragma once

namespace Utils
{
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
            m_prev = NULL
            m_next = NULL
            m_count = 0
            m_lock = 0;
        }

        ~List()
        {
            
        }

    private:
        ListNode<T> *m_prev, *m_next;
        uint32_t m_count;
        volatile int m_lock = 0;
    }
}