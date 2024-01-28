#pragma once

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

        uint64_t find(uint8_t* val, uint64_t size, uint64_t offset = 0x1)
        {
            if (!_size) {
                return 0;
            }
            for (uint64_t i = 0; i < _size; i++) {
                if (((uint8_t*)_base)[i] == *val) {
                    uint64_t j = 0;
                    for (; j < size; j++) {
                        if (((uint8_t*)_base)[i + j] != val[j]) {
                            break;
                        }
                    }
                    if (j == size) {
                        return i;
                    }
                }
            }
            return 0;
        }

        template <typename T>
        uint64_t find(T* val, uint64_t size, uint64_t offset = 0x1)
        {
            return find((uint8_t*)val, size * sizeof(T), offset);
        }

        void clone(AddressRange& other) {}
    };

    template <typename T, typename U>
        requires((Std::IsIntegral<T> or Std::IsPointer<T>) and
                 (Std::IsIntegral<U>))
    AddressRange rangeOf(T base, U size)
    {
        return AddressRange{ ._base = (uint64_t)base, ._size = (uint64_t)size };
    }
}