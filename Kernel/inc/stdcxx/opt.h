#pragma once

#include <stdcxx/func.h>

template <typename T, bool = Std::isTrivial<T>::value>
class Opt
{
public:
    Opt()
      : m_ptr(nullptr)
    {
    }
    Opt(T* ptr)
      : m_ptr(ptr)
    {
    }
    Opt(Opt const& other)
      : m_ptr(other.m_ptr)
    {
    }
    Opt(Opt&& other)
      : m_ptr(other.m_ptr)
    {
        other.m_ptr = nullptr;
    }
    Opt& operator=(const Opt& other)
    {
        if (this != &other) {
            m_ptr = other.m_ptr;
        }
        return *this;
    }
    T*   operator->() const { return m_ptr; }
    T&   operator*() const { return *m_ptr; }
    bool isPresent() const { return m_ptr != nullptr; }
    bool isEmpty() const { return m_ptr == nullptr; }
    T*   get() const { return m_ptr; }
    T*   orElse(T* other) const { return m_ptr ? m_ptr : other; }
    T*   orElseGet(Supplier<T> supplier) const
    {
        return m_ptr ? m_ptr : supplier();
    }
    T* orElseThrow() const
    {
        if (!m_ptr) {
            // TODO: throw exception
        }
        return m_ptr;
    }
    void ifPresent(Consumer<T> consumer) const
    {
        if (m_ptr) {
            consumer(m_ptr);
        }
    }

    static Opt<T> empty() { return Opt<T>(); }

private:
    T* m_ptr;
};

template <typename T>
class Opt<T, true>
{
public:
    Opt()
      : m_value()
      , m_present(false)
    {
    }
    Opt(T value)
      : m_value(value)
      , m_present(true)
    {
    }
    Opt(Opt const& other)
      : m_value(other.m_value)
      , m_present(other.m_present)
    {
    }
    Opt(Opt&& other)
      : m_value(other.m_value)
      , m_present(other.m_present)
    {
        other.m_present = false;
    }
    Opt& operator=(const Opt& other)
    {
        if (this != &other) {
            m_value   = other.m_value;
            m_present = other.m_present;
        }
        return *this;
    }
    T    operator->() const { return m_value; }
    T    operator*() const { return m_value; }
    bool isPresent() const { return m_present; }
    bool isEmpty() const { return !m_present; }
    T    get() const { return m_value; }
    T    orElse(T other) const { return m_present ? m_value : other; }
    T    orElseGet(Supplier<T> supplier) const
    {
        return m_present ? m_value : supplier();
    }
    T orElseThrow() const
    {
        if (!m_present) {
            // TODO: throw exception
        }
        return m_value;
    }
    void ifPresent(Consumer<T> consumer) const
    {
        if (m_present) {
            consumer(m_value);
        }
    }

    static Opt<T> empty() { return Opt<T>(); }

private:
    T    m_value;
    bool m_present;
};