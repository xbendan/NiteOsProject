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

public:
    ArrayList() { }

    ArrayList(uint64_t capacity, bool disableExpand)
      : m_Capacity(capacity),
        m_ExpandDisabled(disableExpand)
    {

    }

    T& operator[](int index)
    {
        return array[index < m_Count ? index : 0];
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

template <typename T, int size> class SizedArrayList
{
private:
    T array[size];
    uint64_t m_Count;
    uint64_t m_Capacity;
    size_t m_ObjectSize = sizeof(T);

public:
    SizedArrayList()
      : m_Count(0),
        m_Capacity(size)
    { }

    T& operator[](int index)
    {
        return array[index < m_Capacity ? index : 0];
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

    size_t Length()
    {
        return m_Count;
    }

    void Add(T obj)
    {
        if (m_Count >= m_Capacity) {
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
};