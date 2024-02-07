#pragma once

#include <stdcxx/type-traits.h>
#include <stdcxx/types.h>

namespace Kern::Hal {
    template <typename T>
        requires Std::isIntegral<T>
    static inline T portRegsRead(uint16_t port)
    {
        T data;
        asm volatile("in %1, %0" : "=a"(data) : "d"(port));
        return data;
    }

    template <typename T>
        requires Std::isIntegral<T>
    static inline void portRegsWrite(uint16_t port, T data)
    {
        asm volatile("out %0, %1" : : "a"(data), "d"(port));
    }

    static inline void waitIO()
    {
        portRegsWrite<uint8_t>(0x80, 0);
    }
}