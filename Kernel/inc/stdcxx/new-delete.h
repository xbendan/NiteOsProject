#pragma once

#include <stdcxx/types.h>

inline void*
operator new(size_t, void* p)
{
    return p;
}

// template <typename T>
// inline void*
// operator new(size_t size, T* p)
// {
//     return p;
// }