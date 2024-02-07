#pragma once

#include <stdcxx/type-traits.h>

template <typename T>
    requires Std::IsArithmetic<T>
inline T
abs(T x)
{
    return x < 0 ? -x : x;
}

template <typename T>
    requires Std::IsArithmetic<T>
inline T
min(T a, T b)
{
    return a < b ? a : b;
}

template <typename T>
    requires Std::IsArithmetic<T>
inline T
max(T a, T b)
{
    return a > b ? a : b;
}

template <typename T, typename U>
    requires Std::IsArithmetic<T> and Std::isIntegral<U>
inline T
pow(T base, U exp)
{
    T result = 1;
    while (exp) {
        if (exp & 1) {
            result *= base;
        }
        exp  >>= 1;
        base  *= base;
    }
    return result;
}
