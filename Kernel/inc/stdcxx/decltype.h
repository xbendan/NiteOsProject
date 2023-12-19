#include <stdcxx/reference.h>

template <typename T>
typename Std::AddRvalueReference<T>::Type
declval() noexcept
{
    return static_cast<typename Std::AddRvalueReference<T>::Type>(
      *((T*)nullptr));
}