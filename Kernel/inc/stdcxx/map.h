#include <stdcxx/hash.h>
#include <stdcxx/type-traits.h>

namespace Std {
    template <class K, class V>
        requires Std::IsBaseOf<IHashCode, K>::Value
    class HashMap
    {};
}