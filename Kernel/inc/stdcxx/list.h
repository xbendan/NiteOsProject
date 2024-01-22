#pragma once

#include <stdcxx/iterator.h>
#include <stdcxx/types.h>

namespace Std {
    template <typename T>
    class List
    {
    public:
        virtual ~List() = default;

        virtual void   add(T& value)      = 0;
        virtual void   remove(uint64_t i) = 0;
        virtual void   remove(T const& t) = 0;
        virtual void   clear()            = 0;
        virtual T*     get(uint64_t i)    = 0;
        virtual size_t size()             = 0;

        virtual T& operator[](uint64_t index) = 0;
    };
}
