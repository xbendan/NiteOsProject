#include <mixins/std/type_traits.h>

template <typename _Tp>
typename AddRvalueReference<_Tp>::type declval() noexcept
{
    return static_cast<typename AddRvalueReference<_Tp>::type>(
      *((_Tp*)nullptr));
}