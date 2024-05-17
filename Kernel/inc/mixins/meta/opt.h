#include <mixins/meta/func.h>
#include <mixins/std/type_traits.h>

template <class _Tp>
    requires(!mixins::std::isTrivial<_Tp>::value)
class Opt
{};

template <class _Tp>
class Opt
{
public:
    Opt()
      : m_hasValue(false)
    {
    }
    Opt(_Tp value)
      : m_value(value)
      , m_hasValue(true)
    {
    }

    bool isPresent() const { return m_hasValue; }

    _Tp get() const { return m_value; }

    _Tp orElse(_Tp defaultValue) const
    {
        return m_hasValue ? m_value : defaultValue;
    }

    _Tp orElseGet(Supplier<_Tp> supplier) const
    {
        return m_hasValue ? m_value : supplier();
    }

    _Tp orElseThrow(Runnable runnable) const
    {
        if (!m_hasValue) {
            runnable();
        }
        return m_value;
    }

    void ifPresent(Consumer<_Tp> consumer) const
    {
        if (m_hasValue) {
            consumer(m_value);
        }
    }

    Opt<_Tp> filter(Predicate<_Tp> predicate) const
    {
        if (m_hasValue && predicate(m_value)) {
            return *this;
        }
        return Opt<_Tp>();
    }

private:
    _Tp  m_value;
    bool m_hasValue;
};