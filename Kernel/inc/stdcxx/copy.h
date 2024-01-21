#pragma once

#include <stdcxx/reference.h>

namespace Std {
    template <typename T>
    constexpr T&& Forward(RemoveReference<T>::Type& p) noexcept
    {
        return static_cast<Tp&&>(p);
    };

    template <typename T>
    constexpr T&& Forward(RemoveReference<T>::Type&& p) noexcept
    {
        return static_cast<Tp&&>(p);
    };

    template <typename T>
    constexpr RemoveReference<T>::Type&& Move(T&& t) noexcept
    {
        return static_cast<RemoveReference<T>::Type&&>(t);
    };
}