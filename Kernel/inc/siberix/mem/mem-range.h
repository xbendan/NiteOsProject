#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

namespace Kern::Mem {
    struct MemRange
    {
        UInt64 _base, _size;

        Void clear() {}

        Void set(UInt8 val) {}

        Void clone(MemRange& other) {}
    };

    template <typename T, typename U>
        requires((Std::IsIntegral<T> or Std::IsPointer<T>) and
                 (Std::IsIntegral<U> or Std::IsPointer<U>))
    MemRange rangeOf(T base, U size)
    {
        return MemRange{ ._base = (UInt64)base, ._size = (UInt64)size };
    }

    template <typename T>
        requires Std::IsIntegral<T> or Std::IsPointer<T>
    MemRange rangeOf(UInt64 base)
    {
        return MemRange{ ._base = (UInt64)base };
    }
}