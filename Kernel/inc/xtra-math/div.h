#include <stdcxx/type-traits.h>

template <typename T, typename U>
    requires Std::IsIntegral<T> and Std::IsIntegral<U>
T
divCeil(T a, T b)
{
    return (a / b) + (a % b != 0);
}

template <typename T>
    requires Std::IsArithmetic<T>
T
divCeil(T a, T b)
{
    return (a / b) + (a % b != 0);
}

template <typename T, typename U>
    requires Std::IsArithmetic<T> and Std::IsArithmetic<U>
T
divFloor(T a, T b)
{
    return a / b;
}