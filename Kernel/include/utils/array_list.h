#pragma once

#include <utils/collection.h>

namespace utils {
    template <typename T>
    class ArrayList : public Collection<T>
    {
    public:
        ArrayList()
          : ArrayList(16)
        {
        }
        ArrayList(unsigned initialCapacity)
          : m_capacity(initialCapacity)
        {
            m_arr = new T[initialCapacity];
        }
        ~ArrayList();

    private:
        T*            m_arr;
        unsigned long m_length;
        unsigned long m_capacity;
    };
}