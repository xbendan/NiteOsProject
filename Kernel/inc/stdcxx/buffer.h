#pragma once

#include <stdcxx/types.h>

namespace Std {
    template <typename T>
    class Buffer
    {
    public:
        Buffer(uint64_t size, uint64_t offset)
          : m_buf(reinterpret_cast<uint64_t>(new T[size]))
          , m_offset(offset)
        {
        }

        T& operator[](uint64_t i) { return *(T*)(m_buf + i * m_offset); }

        Buffer& operator=(uint64_t address) {}

    private:
        uint64_t m_buf;
        uint64_t m_offset;
    };
}