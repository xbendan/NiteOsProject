#include <stdcxx/decltype.h>
#include <stdcxx/types.h>

namespace Std {
    // clang-format off
    template <typename T> constexpr Boolean IsIntegral = false;
    template <> constexpr Boolean IsIntegral<unsigned char> = true;
    template <> constexpr Boolean IsIntegral<unsigned short> = true;
    template <> constexpr Boolean IsIntegral<unsigned int> = true;
    template <> constexpr Boolean IsIntegral<unsigned long> = true;
    template <> constexpr Boolean IsIntegral<unsigned long long> = true;
    template <> constexpr Boolean IsIntegral<char> = true;
    template <> constexpr Boolean IsIntegral<short> = true;
    template <> constexpr Boolean IsIntegral<int> = true;
    template <> constexpr Boolean IsIntegral<long> = true;
    template <> constexpr Boolean IsIntegral<long long> = true;

    template <typename T> constexpr Boolean IsFloatingPoint = false;
    template <> constexpr Boolean IsFloatingPoint<float> = true;
    template <> constexpr Boolean IsFloatingPoint<double> = true;
    template <> constexpr Boolean IsFloatingPoint<long double> = true;

    template <typename T> constexpr Boolean IsArithmetic = IsIntegral<T> || IsFloatingPoint<T>;

    template <typename T1, typename T2> constexpr bool IsSame = false;
    template <typename T1> constexpr bool IsSame<T1, T1> = true;

    template <typename T> constexpr Boolean IsConst = false;
    template <typename T> constexpr Boolean IsConst<const T> = true;

    template <typename T> constexpr Boolean IsVolatile = false;
    template <typename T> constexpr Boolean IsVolatile<volatile T> = true;

    template <typename T> struct RemoveConstant { using Value = T; };
    template <typename T> struct RemoveConstant<const T> { using Value = T; };
    template <typename T> struct RemoveVolatile { using Value = T; };
    template <typename T> struct RemoveVolatile<volatile T> { using Value = T; };
    template <typename T> struct RemoveConstVolatile { using Value = T; };
    template <typename T> struct RemoveConstVolatile<const T> { using Value = T; };
    template <typename T> struct RemoveConstVolatile<volatile T> { using Value = T; };
    template <typename T> struct RemoveConstVolatile<const volatile T> { using Value = T; };

    template <typename T> struct IsVoid : IsSame<void, RemoveConstVolatile<T>::Value> {};
    
    template <typename T> constexpr Boolean IsPointer = false;
    template <typename T> constexpr Boolean IsPointer<T*> = true;

    template <typename T> constexpr Boolean IsArray = false;
    template <typename T> constexpr Boolean IsArray<T[]> = true;
    template <typename T, unsigned N> constexpr Boolean IsArray<T[N]> = true;

    template <typename> constexpr bool IsFunction = false;
    template <typename Ret, typename... Args> constexpr bool IsFunction<Ret(Args...)> = true;
    template <typename Ret, typename... Args> constexpr bool IsFunction<Ret(Args..., ...)> = true;
    template <typename F, typename... Args> concept IsCallable = requires(F f) { f(declval<Args>()...); };

    // clang-format on
}