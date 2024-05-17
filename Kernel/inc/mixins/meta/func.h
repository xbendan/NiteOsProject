#include <mixins/std/type_traits.h>

template <typename>
class Func;

template <typename _Ret, typename... _Args>
class Func<_Ret(_Args...)>
{
public:
    /* --- Constructors --- */
    Func() = delete;
    template <typename _Callable>
        requires isCallable<_Callable, _Args...> and
                   (sizeof(_Callable) > sizeof(_Ret(*)(_Args...)))
    Func(_Callable&& call)
      : m_funcWrap(new FuncWrap<_Callable>(move(call)))
      , m_useWrapper(true)
    {
    }
    template <typename _Callable>
        requires isCallable<_Callable, _Args...> and
                   (sizeof(_Callable) <= sizeof(_Ret(*)(_Args...)))
    Func(_Callable&& call)
      : m_func(reinterpret_cast<_Ret (*)(_Args...)>(call))
      , m_useWrapper(false)
    {
    }

    Func(_Ret (*call)(_Args...))
      : m_useWrapper(false)
    {
        m_func = call;
    }

    ~Func()
    {
        if (m_useWrapper) {
            delete m_funcWrap;
        }
    }

    /* --- Operator Overloads --- */

    template <typename _Callable>
        requires isCallable<_Callable, _Args...>
    Func& operator=(_Callable&& call)
    {
        m_useWrapper = true;
        m_funcWrap   = new FuncWrap<_Callable>(move(call));
        return *this;
    }

    Func& operator=(_Ret (*call)(_Args...))
    {
        m_useWrapper = false;
        m_func       = call;
        return *this;
    }

    _Ret operator()(_Args... args)
    {
        if (m_useWrapper) {
            return (*m_funcWrap)(args...);
        }
        return m_func(args...);
    }

private:
    struct FuncWrapBase
    {
        virtual _Ret operator()(_Args...) = 0;
        virtual ~FuncWrapBase() {}
    };

    template <typename _Callable>
    struct FuncWrap : public FuncWrapBase
    {
        FuncWrap(_Callable&& call)
          : m_call(move(call))
        {
        }

        _Ret operator()(_Args... args) override { return m_call(args...); }

        _Callable m_call;
    };

    bool m_useWrapper;
    union
    {
        _Ret (*m_func)(_Args...);
        FuncWrapBase* m_funcWrap;
    };
};

using Runnable = Func<void()>;

template <class _Tp>
using Predicate = Func<bool(_Tp)>;

template <class _Tp>
using Consumer = Func<void(_Tp)>;

template <class _Tp>
using Supplier = Func<_Tp()>;