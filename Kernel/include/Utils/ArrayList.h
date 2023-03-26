#pragma once

#include <macros>

template <typename T> class ArrayList
{
private:
    T *array;
    uint64_t m_Count;
    uint64_t m_Capacity;
    size_t m_ObjectSize = sizeof(T);
    bool m_ExpandDisabled;
    uint16_t m_ExpandSize;

public:
    ArrayList() { }

    ArrayList(uint64_t capacity, bool disableExpand, uint16_t sizeToExpand = 16)
      : m_Capacity(capacity),
        m_ExpandDisabled(disableExpand),
        m_ExpandSize(sizeToExpand)
    {

    }

    T& operator[](int index)
    {
        return array[index < m_Count ? index : 0];
    }

    void EnsureCapacity()
    {
        
    }

    bool Contains(T& obj)
    {
        for (int i = 0; i < m_Count; i++)
        {
            if (array[i] == obj)
                return true;
        }

        return false;
    }

    bool IsEmpty()
    {
        return !m_Count;
    }

    void Add(T& obj)
    {
        if (m_Count >= m_Capacity) {
            if (m_ExpandDisabled) {
                return;
            }
            //
        }
        array[m_Count++] = obj;
    }

    void Remove(T& obj)
    {

    }

    void Remove(uint64_t index)
    {

    }
};

template <class T, int size> class SizedArrayList
{
private:
    T array[size];
    uint64_t m_Count = 0;
    size_t m_ObjectSize = sizeof(T);

public:
    SizedArrayList()
      : m_Count(0)
    { }

    T& operator[](int index)
    {
        return array[index < size ? index : 0];
    }

    bool Contains(T& obj)
    {
        for (int i = 0; i < m_Count; i++)
        {
            if (array[i] == obj)
                return true;
        }

        return false;
    }

    bool IsEmpty()
    {
        return !m_Count;
    }

    inline size_t Length()
    {
        return m_Count;
    }
    
    inline size_t Capacity()
    {
        return size;
    }

    inline void Add(T obj)
    {
        if (m_Count > size) {
            return;
        }
        array[m_Count++] = obj;
    }

    void Remove(T obj)
    {

    }

    int IndexOf(T obj)
    {
        for (int i = 0; i < m_Count; i++)
        {
            if (array[i] == obj)
            {
                return i;
            }
        }

        return -1;
    }

    void Remove(uint64_t index)
    {

    }

    template <typename F>
    void ForEach(F consumer)
    {
        for (int i = 0; i < m_Count; i++)
        {
            consumer(array[i], i);
        }
    }
};