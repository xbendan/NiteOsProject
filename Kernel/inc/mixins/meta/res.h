#include <mixins/meta/opt.h>

template <class _Tp, class _Err>
union Res
{
public:
    Res()
      : err(_Err())
    {
    }
    Res(_Tp value)
      : value(value)
    {
    }
    Res(_Err err)
      : err(err)
    {
    }
    ~Res() {}

    Opt<_Tp> value;
    unsigned err;

    bool isSuccess() const { return err == 0; }
    bool isFailure() const { return err != 0; }

    constexpr static Res<_Tp, _Err> Ok(_Tp value)
    {
        return Res<_Tp, _Err>(value);
    }
    constexpr static Res<_Tp, _Err> Err(_Err err)
    {
        return Res<_Tp, _Err>(err);
    }
};

template <class _Tp, typename _Err>
Res<_Tp, _Err>::Ok
wrapAsRes(_Tp value)
{
    return Res<_Tp, _Err>(value);
}

template <class _Tp, typename _Err>
Res<_Tp, _Err>::Err
wrapAsRes(_Err err)
{
    return Res<_Tp, _Err>(err);
}