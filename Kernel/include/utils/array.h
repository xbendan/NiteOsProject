#include <common/typedefs.h>

template <typename T>
class ArrayList {
private:
    T* array;
    u64 count;
    u64 objectSize = sizeof(T);

public:
    ArrayList()
        : ArrayList(8) {}

    ArrayList(u64 size)
        : count(0) {
        array = new T[size];
    }

    ~ArrayList() { delete[] array; }
};

template <typename T, int size>
class SizedArrayList {
private:
    T array[size];
    u64 count = 0;
    u64 objectSize = sizeof(T);

public:
    SizedArrayList()
        : count(0) {}

    T& operator[](int index) { return array[index < size ? index : 0]; }

    bool contains(T& obj) {
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

    void add(T obj) {
        if (count > size) {
            return;
        }
        array[count++] = obj;
    }

    void remove(T obj) {}

    void remove(u64 index) {}

    int indexOf(T& obj) {
        for (int i = 0; i < count; i++) {
            if (array[i] == obj) {
                return i;
            }
        }
        return -1;
    }
};