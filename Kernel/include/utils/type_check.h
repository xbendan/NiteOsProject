
#pragma once

namespace utils {
    template <typename Base, typename Derived>
    class Instanceof
    {
        bool result() { return false; }
    };

    template <typename Base>
    class Instanceof<Base, Base>
    {
        bool result() { return true; }
    };
}