#pragma once

#include <utils/functions/function.h>

namespace utils::function {
    template <typename T>
    class Consumer : public Function<void(T)>
    {
    public:
        Consumer(void (*functor)(T)) {}
        ~Consumer() = default;
    };
}