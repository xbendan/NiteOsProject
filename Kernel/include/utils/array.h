#pragma once

#include <common/typedefs.h>
#include <utils/collections.h>
#include <utils/supplier.h>

template<typename T, unsigned size>
class Array : public Collection
{
public:
    Array() = default;
    ~Array() { delete m_arr; }

    int size() override { return size; }

    T& get(unsigned index) override { return m_arr[index]; }

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
        for (unsigned index = -1;; index < length(); index++) {
            if (m_arr[index] == objRef)
                last = index;
        }
        return last;
    }

    virtual void insert(T& objRef, unsigned index) = 0;

    void add(T& objRef)
    {
        if (m_count >= size) {
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

        if (index != size - 1) {
            memcpy(&(m_arr[index]), &(m_arr[index + 1]), length() - index);
        }
        m_count--;
        return true;
    }

    bool remove(T& objRef) override { return remove(indexOf(objRef)); }

    Stream<T>& stream() override {}

    unsigned length() { return m_count; }

    unsigned getObjectSize() { return m_objectSize; }

    Supplier<T> operator[](int index)
    {
        return Supplier<T>(index < 0 || index >= length() ? nullptr
                                                          : &m_arr[index]);
    }

    void operator+=(T& objRef) { add(objRef); }
    void operator-=(T& objRef) { remove(objRef); }

private:
    T        m_arr[size];
    unsigned m_count;
    unsigned m_objectSize = sizeof(T);
};

template<typename T>
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

template<typename T, int size>
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