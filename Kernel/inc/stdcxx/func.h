#pragma once

#include <stdcxx/move.h>
#include <stdcxx/reference.h>
#include <stdcxx/type-traits.h>

template <typename>
class Function;

template <typename Ret, typename... Args>
class Function<Ret(Args...)>
{
public:
    Function() = delete;
    template <typename Callable>
        requires Std::IsCallable<Callable, Args...> and
                   (sizeof(Callable) > sizeof(Ret(*)(Args...)))
    Function(Callable&& call)
      : m_funcWrap(new FuncWrap<Callable>(Std::move(call)))
      , m_useWrapper(true)
    {
    }
    // template <typename Callable>
    //     requires Std::IsCallable<Callable, Args...> and
    //                Std::IsRvalueReference<Callable&&>
    // Function(Callable&& call)
    //   : m_useWrapper(true)
    //   , m_funcWrap(new FuncWrap<Callable>(Std::move<Callable>(call)))
    // {
    // }
    template <typename Callable>
        requires Std::IsCallable<Callable, Args...> and
                   (sizeof(Callable) <= sizeof(Ret(*)(Args...)))
    Function(Callable&& call)
      : m_func(reinterpret_cast<Ret (*)(Args...)>(call))
      , m_useWrapper(false)
    {
    }

    Function(Ret (*call)(Args...))
      : m_useWrapper(false)
    {
        m_func = call;
    }

    template <typename Callable>
        requires Std::IsCallable<Callable, Args...>
    Function& operator=(Callable&& call)
    {
        m_useWrapper = true;
        m_funcWrap   = new FuncWrap<Callable>(Move<Callable>(call));
        return *this;
    }
    ~Function()
    {
        if (m_useWrapper) {
            delete m_funcWrap;
        }
    }

    Function& operator=(Ret (*call)(Args...))
    {
        m_useWrapper = false;
        m_func       = call;
        return *this;
    }

    Ret operator()(Args... args)
    {
        if (m_useWrapper) {
            return (*m_funcWrap)(args...);
        } else {
            return m_func(args...);
        }
    }

private:
    struct IFuncWrap
    {
        virtual ~IFuncWrap()            = default;
        virtual Ret operator()(Args...) = 0;
    };
    template <typename Callable>
    struct FuncWrap : IFuncWrap
    {
        Callable _call;
        FuncWrap(Callable&& call)
          : _call(call)
        {
        }
        Ret operator()(Args... args)
        {
            return _call(Std::forward<Args>(args)...);
        }
    };
    union
    {
        Ret (*m_func)(Args...);
        IFuncWrap* m_funcWrap;
    };
    bool m_useWrapper;
};

using Runnable = Function<void()>;

template <typename T>
using Predicate = Function<bool(T)>;

template <typename T>
using Consumer = Function<void(T)>;

template <typename T>
using Supplier = Function<T()>;
