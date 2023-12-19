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
    constexpr Boolean IsLvalueReference = false;

    template <typename T>
    constexpr Boolean IsLvalueReference<T&> = true;

    template <typename T>
    concept LvalueReference = IsLvalueReference<T>;

    template <typename>
    constexpr Boolean IsRvalueReference = false;

    template <typename T>
    constexpr Boolean IsRvalueReference<T&&> = true;

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
    struct AddRvalueReference<Void>
    {
        using Type = void;
    };

    template <>
    struct AddRvalueReference<const Void>
    {
        using Type = const void;
    };

    template <>
    struct AddRvalueReference<volatile Void>
    {
        using Type = volatile void;
    };

    template <>
    struct AddRvalueReference<const volatile Void>
    {
        using Type = const volatile void;
    };
}