#include <stdcxx/move.h>

template <typename T>
class Box
{
public:
    constexpr Box() = delete;
    constexpr Box(T* ptr)
      : _ptr(ptr)
    {
    }
    constexpr ~Box() { delete _ptr; }

    constexpr T* operator->() { return _ptr; }
    constexpr T& operator*() { return *_ptr; }
    auto         operator[](size_t index) { return (*_ptr)[index]; }

private:
    T* _ptr;
};

template <typename T, typename... Args>
constexpr static Box<T>
wrapAsBox(Args&&... args)
{
    return { new T() };
}