#pragma once

#include <stdcxx/types.h>

namespace Std {
    template <typename T>
    struct RemoveReference
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveReference<T&>
    {
        using Type = T;
    };

    template <typename T>
    struct RemoveReference<T&&>
    {
        using Type = T;
    };

    template <typename T>
    constexpr bool IsLvalueReference = false;

    template <typename T>
    constexpr bool IsLvalueReference<T&> = true;

    template <typename T>
    concept LvalueReference = IsLvalueReference<T>;

    template <typename>
    constexpr bool IsRvalueReference = false;

    template <typename T>
    constexpr bool IsRvalueReference<T&&> = true;

    template <typename T>
    concept RvalueReference = IsRvalueReference<T>;

    template <typename T>
    struct AddRvalueReference
    {
        using Type = T&&;
    };

    template <typename T>
    struct AddRvalueReference<T&>
    {
        using Type = T&;
    };

    template <>
    struct AddRvalueReference<void>
    {
        using Type = void;
    };

    template <>
    struct AddRvalueReference<const void>
    {
        using Type = const void;
    };

    template <>
    struct AddRvalueReference<volatile void>
    {
        using Type = volatile void;
    };

    template <>
    struct AddRvalueReference<const volatile void>
    {
        using Type = const volatile void;
    };
}