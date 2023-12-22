#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

namespace Kern::Mem {
    struct AddressRange
    {
        uint64_t _base, _size;

        void clear() { set(0); }

        void set(uint8_t val)
        {
            if (!_size) {
                return;
            }
            for (uint64_t i = 0; i < _size; i++) {
                ((uint8_t*)_base)[i] = val;
            }
        }

        void clone(AddressRange& other) {}
    };

    template <typename T, typename U>
        requires((Std::IsIntegral<T> or Std::IsPointer<T>) and
                 (Std::IsIntegral<U> or Std::IsPointer<U>))
    AddressRange rangeOf(T base, U size)
    {
        return AddressRange{ ._base = (uint64_t)base, ._size = (uint64_t)size };
    }

    template <typename T>
        requires Std::IsIntegral<T> or Std::IsPointer<T>
    AddressRange rangeOf(uint64_t base)
    {
        return AddressRange{ ._base = (uint64_t)base };
    }
}