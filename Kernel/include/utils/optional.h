#pragma once

#include <utils/functions/consumer.h>

namespace utils {
    template <typename T>
    class Optional
    {
    public:
        Optional() = default;
        Optional(T const* objPtr)
          : m_objPtr(const_cast<T*>(objPtr))
        {
        }
        ~Optional() {}

        bool isEmpty() { return m_objPtr == nullptr; }
        bool isPresent() { return m_objPtr != nullptr; }
        T*   get() { return m_objPtr; }
        T*   orElse(T* fallback) { return isPresent() ? m_objPtr : fallback; }
        void ifPresent(utils::function::Consumer<T&> consumer)
        {
            if (isPresent()) {
                consumer(*m_objPtr);
            }
        }

        bool operator==(const Optional<T>& val)
        {
            return m_objPtr == val.m_objPtr;
        }

    private:
        T* m_objPtr;
    };
}