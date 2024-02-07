#pragma once

#include <stdcxx/type-traits.h>

// Align the nearest _lower_ aligned address
// ex: 8 with align = 8 -> 8
// ex: 9 with align = 8 -> 16
// ex: 7 with align = 8 -> 0
template <typename T, typename U>
    requires Std::isIntegral<T> and Std::isIntegral<U>
static inline T
alignDown(T _addr, U _align)
{
    return (_addr) & ~(_align - 1);
}

// Align the nearest _upper_ aligned address
// ex: 8 with align = 8 -> 8
// ex: 9 with align = 8 -> 16
// ex: 7 with align = 8 -> 8
template <typename T, typename U>
    requires Std::isIntegral<T> and Std::isIntegral<U>
static inline T
alignUp(T _addr, U _align)
{
    return ((_addr + _align) - 1) & ~(_align - 1);
}

template <typename T>
    requires Std::isIntegral<T>
static inline T
alignDownRef(T& _addr, T _align)
{
    return (_addr = alignDown(_addr, _align));
}

template <typename T>
    requires Std::isIntegral<T>
static inline T
alignUpRef(T& _addr, T _align)
{
    return (_addr = alignUp(_addr, _align));
}

template <typename T>
    requires Std::isIntegral<T>
static inline T
alignTwoExponent(T x)
{
    x--;
    x        |= x >> 1;
    x        |= x >> 2;
    x        |= x >> 4;
    x        |= x >> 8;
    x        |= x >> 16;
    x        |= x >> 32;
    return x += 1;
}