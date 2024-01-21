#pragma once

#include <stdcxx/decltype.h>
#include <stdcxx/types.h>

namespace Std {
    // clang-format off
    template <class T, T t>
    struct IntegralConstant {
        static constexpr T Value = t;
        using ValueType = T;
        using Type = IntegralConstant<T, t>;
        constexpr operator ValueType() const noexcept { return Value; }
        constexpr ValueType operator()() const noexcept { return Value; }
    };

    struct TrueType {
        static constexpr bool Value = true;
    };

    struct FalseType {
        static constexpr bool Value = false;
    };

    template <typename T> constexpr bool IsIntegral = false;
    template <> constexpr bool IsIntegral<unsigned char> = true;
    template <> constexpr bool IsIntegral<unsigned short> = true;
    template <> constexpr bool IsIntegral<unsigned int> = true;
    template <> constexpr bool IsIntegral<unsigned long> = true;
    template <> constexpr bool IsIntegral<unsigned long long> = true;
    template <> constexpr bool IsIntegral<char> = true;
    template <> constexpr bool IsIntegral<short> = true;
    template <> constexpr bool IsIntegral<int> = true;
    template <> constexpr bool IsIntegral<long> = true;
    template <> constexpr bool IsIntegral<long long> = true;

    template <typename T> constexpr bool IsFloatingPoint = false;
    template <> constexpr bool IsFloatingPoint<float> = true;
    template <> constexpr bool IsFloatingPoint<double> = true;
    template <> constexpr bool IsFloatingPoint<long double> = true;

    template <typename T> constexpr bool IsArithmetic = IsIntegral<T> || IsFloatingPoint<T>;

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

    template <typename T>
    using IsVoid = IsSame<void, RemoveConstVolatile<T>::Value>;
    
    template <typename T> constexpr bool IsPointer = false;
    template <typename T> constexpr bool IsPointer<T*> = true;

    template <typename T> constexpr bool IsArray = false;
    template <typename T> constexpr bool IsArray<T[]> = true;
    template <typename T, unsigned N> constexpr bool IsArray<T[N]> = true;

    template <typename> constexpr bool IsFunction = false;
    template <typename Ret, typename... Args> constexpr bool IsFunction<Ret(Args...)> = true;
    template <typename Ret, typename... Args> constexpr bool IsFunction<Ret(Args..., ...)> = true;
    template <typename F, typename... Args> concept IsCallable = requires(F f) { f(declval<Args>()...); };

    template <typename T>
    struct IsClass : IntegralConstant<bool, __is_class(T)> {};

    template <typename T>
    struct IsUnion : IntegralConstant<bool, __is_union(T)> {};

    template <typename T>
    struct IsEnum : IntegralConstant<bool, __is_enum(T)> {};

    namespace _ {
        template <typename> 
        FalseType TestPtrConversion(const volatile void*);
        template <typename T>
        TrueType TestPtrConversion(const volatile T*);

        template <typename, typename>
        auto TestIsBaseOf(...) -> TrueType;
        template <typename Base, typename Derived>
        auto TestIsBaseOf(int) -> decltype(TestPtrConversion<Base>(declval<Derived*>(nullptr)));
    }

    // template <typename B, typename D>
    // using IsBaseOf = 
    //     IsClass<B>::Value && 
    //     IsClass<D>::Value &&
    //     decltype(_::TestIsBaseOf<B, D>(0));
    
    template <typename B, typename D>
    struct IsBaseOf : IntegralConstant<bool, __is_base_of(B, D)> {};

    // clang-format on
}