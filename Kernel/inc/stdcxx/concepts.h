#pragma once

#include <stdcxx/decltype.h>
#include <stdcxx/types.h>

namespace Std {
    template <typename T>
    constexpr Boolean IsIntegral = false;

    template <>
    constexpr Boolean IsIntegral<unsigned char> = true;

    template <>
    constexpr Boolean IsIntegral<unsigned short> = true;

    template <>
    constexpr Boolean IsIntegral<unsigned int> = true;

    template <>
    constexpr Boolean IsIntegral<unsigned long> = true;

    template <>
    constexpr Boolean IsIntegral<unsigned long long> = true;

    template <>
    constexpr Boolean IsIntegral<char> = true;

    template <>
    constexpr Boolean IsIntegral<short> = true;

    template <>
    constexpr Boolean IsIntegral<int> = true;

    template <>
    constexpr Boolean IsIntegral<long> = true;

    template <>
    constexpr Boolean IsIntegral<long long> = true;

    template <typename T>
    constexpr Boolean IsPointer = false;

    template <typename T>
    constexpr Boolean IsPointer<T*> = true;

    template <typename T>
    concept Integral = IsIntegral<T>;

    template <typename A, typename B>
    constexpr bool IsSame = false;

    template <typename A>
    constexpr bool IsSame<A, A> = true;

    template <typename A, typename B>
    concept Same = IsSame<A, B>;

    template <typename>
    constexpr bool IsFunction = false;

    template <typename Ret, typename... Args>
    constexpr bool IsFunction<Ret(Args...)> = true;

    template <typename Ret, typename... Args>
    constexpr bool IsFunction<Ret(Args..., ...)> = true;

    template <typename F, typename... Args>
    concept IsCallable = requires(F f) { f(declval<Args>()...); };
}