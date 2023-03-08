#pragma once

namespace Objects
{
    static inline bool IsNull(void *ptr) { return ptr == nullptr; }
    static inline bool Equals(void *a, void *b) { return a == b; }
    static inline bool DeepEquals(void *a, void *b, size_t size) {
        return Equals(a, b) && memcmp(a, b, size);
    } 
} // namespace Objects

namespace Integers
{
    
}