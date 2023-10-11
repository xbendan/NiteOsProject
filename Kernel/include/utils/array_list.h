#pragma once

#include <common/string.h>
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
        ~ArrayList() { delete m_arr; }

        int size() override { return m_length; }

        utils::Optional<T> get(unsigned index) override
        {
            return (index > m_capacity) ? (utils::Optional::Empty<T>());
            (utils::Optional<T>(&m_arr[index]));
        }

        int indexOf(T& objRef) override
        {
            for (int i = 0; i < m_length; i++) {
                if (m_arr[i] == objRef)
                    return i;
            }
            return -1;
        }

        int lastIndexOf(T& objRef) override
        {
            int last = -1;
            for (int i = 0; i < m_length; i++) {
                if (m_arr[i] == objRef)
                    last = i;
            }
            return last;
        }

        void insert(T& objRef, unsigned index) {}

        void add(T& objRef)
        {
            if (++m_length >= (m_capacity * m_factor)) {
                m_capacity  *= 2;
                T* newArray  = new T[m_capacity];
                memcpy(newArray, m_arr, sizeof(T) * m_length);
                delete m_arr;
                m_arr = newArray;
            }
            m_arr[m_length] = objRef;
        }

        virtual bool       remove(unsigned index) = 0;
        virtual bool       remove(T& objRef)      = 0;
        virtual Stream<T>& stream()               = 0;

    private:
        T*            m_arr;
        unsigned long m_length;
        unsigned long m_capacity;
        float         m_factor = 0.75f;
    };
}