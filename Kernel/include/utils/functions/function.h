#pragma once

#include <utils/tuple.h>

namespace utils::function {
    // template <typename Callable>
    // class FunctionTraits;

    // template <typename ReturnType, typename Argument>
    // class FunctionTraits<ReturnType (*)(Argument&)>
    // {
    //     using _ReturnType = ReturnType;
    //     using _Argument   = Argument;
    // };

    // template <typename Callable>
    // class Function
    // {
    //     using ReturnType = typename FunctionTraits<Callable>::_ReturnType;
    //     using Argument   = typename FunctionTraits<Callable>::_Argument;

    // public:
    //     Function(Callable&& callable)
    //       : m_callable(callable)
    //     {
    //     }
    //     ~Function() = default;

    //     ReturnType operator()(Argument& argument)
    //     {
    //         return m_callable(argument);
    //     }

    // private:
    //     Callable m_callable;
    // };

    template <typename>
    class Function;

    template <typename ReturnType, typename Argument>
    class Function<ReturnType(Argument)>
    {
        // template <typename T>
        // struct Callable
        // {
        //     T m_functor;
        //     Callable(T functor)
        //       : m_functor(functor)
        //     {
        //     }

        //     ReturnType operator()(Argument arg) { return f(arg); }
        // } callable;
    public:
        Function(ReturnType (*functor)(Argument))
          : m_functor(functor)
        {
        }
        ~Function() {}

        ReturnType operator()(Argument& arg) { return m_functor(arg); }

    private:
        ReturnType (*m_functor)(Argument);
    }
}