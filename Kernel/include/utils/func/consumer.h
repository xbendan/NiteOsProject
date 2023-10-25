#pragma once

#include <utils/func/function.h>

namespace utils::func {
    template <typename T>
    class Consumer : public Function<void(T)>
    {
    public:
        template <
          typename Callable,
          typename = typename IsEnabled<utils::Instanceof<
            decltype(utils::declval<Callable>()(utils::declval<T>()...)),
            void>::value>::type>
        Consumer(Callable&& callable)
          : Function(callable)
        {
        }
        ~Consumer() = default;
    };
}