#include <stdcxx/type-traits.h>
#include <stdcxx/reference.h>

namespace Std {
    template <typename>
    class Function;

    template <typename Ret, typename... Args>
    class Function<Ret(Args...)>
    {
    private:
        struct FuncWrap
        {
            virtual ~FuncWrap()             = default;
            virtual Ret operator()(Args...) = 0;
        };
        template <typename Call>
        struct FuncWrapImpl : FuncWrap
        {
            Call _call;
            FuncWrapImpl(Call&& call)
              : _call(call)
            {
            }
            virtual ~FuncWrapImpl() = default;
            Ret operator()(Args... args) override
            {
                return _call(Forward<Args>(args)...);
            }
        };

        FuncWrap* _funcWrap;

    public:
        Function() = delete;

        template <typename Func>
            requires IsCallable<Func, Args...>
        Function(Func func)
          : _funcWrap(new FuncWrapImpl<Func>(Move<Func>(func)))
        {
        }

        template <typename Func>
            requires IsRValueRef<Func&&> and IsCallable<Func, Args...>
        Function(Func&& func)
          : _funcWrap(new FuncWrapImpl<Func>(Move<Func>(func)))
        {
        }

        template <typename Func>
            requires IsCallable<Func, Args...>
        Function& operator=(Func func)
        {
            _funcWrap = new FuncWrapImpl<Func>(Move<Func>(func));
            return *this;
        }

        template <typename Func>
            requires IsRValueRef<Func&&> && IsCallable<Func, Args...>
        Function& operator=(Func&& func)
        {
            _funcWrap = new FuncWrapImpl<Func>(Move<Func>(func));
            return *this;
        }

        Ret operator()(Args... args) { return (*_funcWrap)(args...); }
    };
}