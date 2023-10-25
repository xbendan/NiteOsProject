#pragma once

#include <utils/type_check.h>

namespace utils::func {
    template <typename>
    class Function;

    template <typename ReturnType, typename... Arguments>
    class Function<ReturnType(Arguments...)>
    {
    public:
        template <typename Callable>
        Function(Callable&& callable,
                 typename IsEnabled<
                   utils::Instanceof<decltype(utils::declval<Callable>()(
                                       utils::declval<Arguments>()...)),
                                     ReturnType>::value>::type)
          : m_callable(
              new CallableWrapper<typename utils::declval<Callable>::type>(
                callable))
        {
        }

        ReturnType operator()(Arguments... args)
        {
            static_assert(m_callable);
            return m_callable->call(args...);
        }

    private:
        struct CallableBase
        {
            virtual ~CallableBase() {}
            virtual ReturnType call(Arguments... args) = 0;
        };

        template <typename Callable>
        struct CallableWrapper : CallableBase
        {
            CallableWrapper(const Callable& callable)
              : m_callable(callable)
            {
            }

            virtual ReturnType call(Arguments... args) override
            {
                return m_callable(args...);
            }

            Callable m_callable;
        };

        CallableBase* m_callable = nullptr;

        ReturnType (*m_invoker)(void* p, Arguments arg) = nullptr;
    };
}