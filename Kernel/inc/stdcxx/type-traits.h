#pragma once

#include <stdcxx/decltype.h>
#include <stdcxx/types.h>

namespace Std {
    // clang-format off
    template <class T, T t>
    struct Meta {
        static constexpr T value = t;
        using ValueType = T;
        using Type = Meta<T, t>;
        constexpr operator ValueType() const noexcept { return value; }
        constexpr ValueType operator()() const noexcept { return value; }
    };

    using True = Meta<bool, true>;
    using False = Meta<bool, false>;

    template <typename T>
    struct TypeIdentity {
        using type = T;
    };

    template <typename T, size_t = sizeof(T)>
    constexpr True __isCompleteOrUnbounded(TypeIdentity<T>) {
        return {};
    }

    template <typename T> constexpr bool isIntegral = false;
    template <> constexpr bool isIntegral<unsigned char> = true;
    template <> constexpr bool isIntegral<unsigned short> = true;
    template <> constexpr bool isIntegral<unsigned int> = true;
    template <> constexpr bool isIntegral<unsigned long> = true;
    template <> constexpr bool isIntegral<unsigned long long> = true;
    template <> constexpr bool isIntegral<char> = true;
    template <> constexpr bool isIntegral<short> = true;
    template <> constexpr bool isIntegral<int> = true;
    template <> constexpr bool isIntegral<long> = true;
    template <> constexpr bool isIntegral<long long> = true;

    template <typename T> constexpr bool isFloatingPoint = false;
    template <> constexpr bool isFloatingPoint<float> = true;
    template <> constexpr bool isFloatingPoint<double> = true;
    template <> constexpr bool isFloatingPoint<long double> = true;

    template <typename T> constexpr bool IsArithmetic = isIntegral<T> || isFloatingPoint<T>;

    template <typename T1, typename T2> constexpr bool IsSame = false;
    template <typename T1> constexpr bool IsSame<T1, T1> = true;

    template <typename T1, typename T2> concept SameAs = IsSame<T1, T2>;

    template <typename T> constexpr bool IsConst = false;
    template <typename T> constexpr bool IsConst<const T> = true;

    template <typename T> constexpr bool IsVolatile = false;
    template <typename T> constexpr bool IsVolatile<volatile T> = true;

    template <typename T> struct RemoveConstant { using Value = T; };
    template <typename T> struct RemoveConstant<const T> { using Value = T; };
    template <typename T> struct RemoveVolatile { using Value = T; };
    template <typename T> struct RemoveVolatile<volatile T> { using Value = T; };
    template <typename T> struct RemoveConstVolatile { using Value = T; };
    template <typename T> struct RemoveConstVolatile<const T> { using Value = T; };
    template <typename T> struct RemoveConstVolatile<volatile T> { using Value = T; };
    template <typename T> struct RemoveConstVolatile<const volatile T> { using Value = T; };

    template <typename T> constexpr bool isVoid = false;
    template <> constexpr bool isVoid<void> = true;
    
    template <typename T>
    struct isPointer : Meta<bool, false> {};

    template <typename T>
    struct isPointer<T*> : Meta<bool, true> {};
    
    template <typename T>
    struct RemovePointer { using Type = T; };

    template <typename T>
    struct RemovePointer<T*> { using Type = T; };

    template <typename T>
    struct RemovePointer<T* const> { using Type = T; };

    template <typename T>
    struct RemovePointer<T* volatile> { using Type = T; };

    template <typename T>
    struct RemovePointer<T* const volatile> { using Type = T; };

    template <typename T> constexpr bool IsArray = false;
    template <typename T> constexpr bool IsArray<T[]> = true;
    template <typename T, unsigned N> constexpr bool IsArray<T[N]> = true;

    template <typename> constexpr bool IsFunction = false;
    template <typename Ret, typename... Args> constexpr bool IsFunction<Ret(Args...)> = true;
    template <typename Ret, typename... Args> constexpr bool IsFunction<Ret(Args..., ...)> = true;
    template <typename F, typename... Args> concept IsCallable = requires(F f) { f(declval<Args>()...); };

    template <typename T>
    struct isClass : Meta<bool, __is_class(T)> {};

    template <typename T>
    struct isUnion : Meta<bool, __is_union(T)> {};

    template <typename T>
    struct isEnum : Meta<bool, __is_enum(T)> {};

    template <typename T>
    struct isTrivial : Meta<bool, __is_trivial(T)>
    {
        static_assert(__isCompleteOrUnbounded(TypeIdentity<T>{}), 
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T, bool = isTrivial<T>::value>
    struct Trivial {
        using type = T*;
    };

    template <typename T>
    struct Trivial<T, true> {
        using type = T;
    };

    template <typename T>
    struct isTriviallyCopyable : Meta<bool, __is_trivially_copyable(T)>
    {
        static_assert(__isCompleteOrUnbounded(TypeIdentity<T>{}), 
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    struct isStandardLayout : Meta<bool, __is_standard_layout(T)>
    {
        static_assert(__isCompleteOrUnbounded(TypeIdentity<T>{}), 
            "template argument must be a complete class or an unbounded array");
    };

    // template <typename B, typename D>
    // using IsBaseOf = 
    //     IsClass<B>::Value && 
    //     IsClass<D>::Value &&
    //     decltype(_::TestIsBaseOf<B, D>(0));
    
    template <typename B, typename D>
    struct isBaseOf : Meta<bool, __is_base_of(B, D)> {};

    template <typename B, typename D>
    concept Derived = isBaseOf<B, D>::Value;

    template <typename T, typename U>
    struct isConvertible : Meta<bool, __is_convertible(T, U)> {};

    // clang-format on
}