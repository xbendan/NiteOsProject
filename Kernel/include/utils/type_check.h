
#pragma once

namespace utils {
    template <typename T>
    T&& declval() noexcept;

    template <typename Base, typename Derived>
    class Instanceof
    {
        static const bool value = false;
        bool              result() { return false; }
    };

    template <typename Base>
    class Instanceof<Base, Base>
    {
        static const bool value = true;
        bool              result() { return true; }
    };

    template <bool Condition, typename T = void>
    class IsEnabled
    {};

    template <typename T>
    class IsEnabled<true, T>
    {
        using Type = T;
    };
}