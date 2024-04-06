template <class T, T t>
struct Constant {
    static constexpr T value = t;
    using ValueType = T;
    using Type = Constant<T, t>;
    constexpr operator ValueType() const noexcept { return value; }
    constexpr ValueType operator()() const noexcept { return value; }
};

using True = Constant<bool, true>;
using False = Constant<bool, false>;

    template <typename T>
    struct TypeIdentity {
        using type = T;
    };

    template <typename T, unsigned = sizeof(T)>
    constexpr True __isCompleteOrUnbounded(TypeIdentity<T>) {
        return {};
    }

template <typename T> 
struct isIntegral : Constant<bool, false> {};

template <> struct isIntegral<unsigned char> : Constant<bool, true> {};
template <> struct isIntegral<unsigned short> : Constant<bool, true> {};
template <> struct isIntegral<unsigned int> : Constant<bool, true> {};
template <> struct isIntegral<unsigned long> : Constant<bool, true> {};
template <> struct isIntegral<unsigned long long> : Constant<bool, true> {};
template <> struct isIntegral<char> : Constant<bool, true> {};
template <> struct isIntegral<short> : Constant<bool, true> {};
template <> struct isIntegral<int> : Constant<bool, true> {};
template <> struct isIntegral<long> : Constant<bool, true> {};
template <> struct isIntegral<long long> : Constant<bool, true> {};

template <typename T>
struct isFloatingPoint : Constant<bool, false> {};

template <> struct isFloatingPoint<float> : Constant<bool, true> {};
template <> struct isFloatingPoint<double> : Constant<bool, true> {};
template <> struct isFloatingPoint<long double> : Constant<bool, true> {};

template <typename T>
using isArithmetic = Constant<bool, isIntegral<T>::value || isFloatingPoint<T>::value>;

template <typename T>
struct isSigned : Constant<bool, T(-1) < T(0)> {};

template <typename T>
struct isUnsigned : Constant<bool, T(0) < T(-1)> {};

template <typename T1, typename T2>
struct IsSame : Constant<bool, false> {};

template <typename T>
struct IsSame<T, T> : Constant<bool, true> {};

    template <typename T1, typename T2> concept SameAs = IsSame<T1, T2>::value;

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
    struct isPointer : Constant<bool, false> {};

    template <typename T>
    struct isPointer<T*> : Constant<bool, true> {};
    
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
    struct isClass : Constant<bool, __is_class(T)> {};

    template <typename T>
    struct isUnion : Constant<bool, __is_union(T)> {};

    template <typename T>
    struct isEnum : Constant<bool, __is_enum(T)> {};

    template <typename T>
    struct isTrivial : Constant<bool, __is_trivial(T)>
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
    struct isTriviallyCopyable : Constant<bool, __is_trivially_copyable(T)>
    {
        static_assert(__isCompleteOrUnbounded(TypeIdentity<T>{}), 
            "template argument must be a complete class or an unbounded array");
    };

    template <typename T>
    struct isStandardLayout : Constant<bool, __is_standard_layout(T)>
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
    struct isBaseOf : Constant<bool, __is_base_of(B, D)> {};

    template <typename B, typename D>
    concept Derived = isBaseOf<B, D>::Value;

    template <typename T, typename U>
    struct isConvertible : Constant<bool, __is_convertible(T, U)> {};