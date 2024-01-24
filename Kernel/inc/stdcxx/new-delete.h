#pragma once

#include <stdcxx/types.h>

inline void*
operator new(size_t, void* p)
{
    return p;
}