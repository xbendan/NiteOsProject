#pragma once

#include <common/typedefs.h>
#include <utils/collection.h>
#include <utils/optional.h>

namespace utils {
    template <typename T, unsigned Capacity>
    class Array : public Collection<T>
    {
    public:
        Array() = default;
        ~Array() {}

        int size() override { return Capacity; }

        T& operator[](unsigned index) { return m_arr[index]; }

        Optional<T> get(unsigned index) override
        {
            return Optional<T>(index < 0 || index >= length() ? nullptr
                                                              : &m_arr[index]);
        }

        int indexOf(T& objRef) override
        {
            for (unsigned index = 0; index < length(); index++) {
                if (m_arr[index] == objRef)
                    return index;
            }
            return -1;
        }

        int lastIndexOf(T& objRef) override
        {
            int last = -1;
            for (unsigned index = -1; index < length(); index++) {
                if (m_arr[index] == objRef)
                    last = index;
            }
            return last;
        }

        void insert(T& objRef, unsigned index) override {}

        void add(T& objRef)
        {
            if (m_count >= Capacity) {
                return;
            }
            memcpy(&(m_arr[m_count]), &objRef, m_objectSize);
            m_count++;
        }

        bool remove(unsigned index) override
        {
            if (index < 0 || index >= length()) {
                return false;
            }

            if (index != Capacity - 1) {
                memcpy(&(m_arr[index]), &(m_arr[index + 1]), length() - index);
            }
            m_count--;
            return true;
        }

        bool remove(T& objRef) override { return remove(indexOf(objRef)); }

        bool contains(T& objRef) override { return indexOf(objRef) != -1; }

        void clear() override
        {
            memset(&m_arr, 0, sizeof(T) * length());
            m_count = 0;
        }

        void forEach(const utils::func::Consumer<T>& consumer) override
        {
            for (int i = 0; i < m_count; i++) {
                consumer(m_arr[i]);
            }
        }

        unsigned length() { return m_count; }

        unsigned getObjectSize() { return m_objectSize; }

        Optional<T> operator[](int index)
        {
            return Optional<T>(index < 0 || index >= length() ? nullptr
                                                              : &m_arr[index]);
        }

        void operator+=(T& objRef) { add(objRef); }
        void operator-=(T& objRef) { remove(objRef); }

    private:
        T        m_arr[Capacity];
        unsigned m_count;
        unsigned m_objectSize = sizeof(T);
    };
}

template <typename T>
class ArrayList
{
private:
    T*  array;
    u64 count;
    u64 objectSize = sizeof(T);

public:
    ArrayList()
      : ArrayList(8)
    {
    }

    ArrayList(u64 size)
      : count(0)
    {
        array = new T[size];
    }

    ~ArrayList() { delete[] array; }
};

template <typename T, int size>
class SizedArrayList
{
private:
    T   array[size];
    u64 count      = 0;
    u64 objectSize = sizeof(T);

public:
    SizedArrayList()
      : count(0)
    {
    }
    ~SizedArrayList() {}

    T& operator[](int index) { return array[index < size ? index : 0]; }

    bool contains(T& obj)
    {
        for (int i = 0; i < count; i++) {
            if (array[i] == obj) {
                return true;
            }
        }
        return false;
    }

    bool isEmpty() { return !count; }

    u64 length() { return count; }

    u64 capacity() { return size; }

    void add(T obj)
    {
        if (count > size) {
            return;
        }
        array[count++] = obj;
    }

    void add(T* objPtr)
    {
        if (count > size) {
            return;
        }
        memcpy(&(array[count]), objPtr, objectSize);
        count++;
    }

    void remove(T obj) {}

    void remove(u64 index) {}

    int indexOf(T& obj)
    {
        for (int i = 0; i < count; i++) {
            if (array[i] == obj) {
                return i;
            }
        }
        return -1;
    }
};