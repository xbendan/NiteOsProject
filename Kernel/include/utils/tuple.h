#pragma once

namespace utils {
    template <typename... Args>
    class Tuple;

    template <typename T>
    class Tuple<T>
    {
    public:
        Tuple(T value)
          : m_value(value)
        {
        }
        ~Tuple() = default;

        T get() const { return m_value; }

    private:
        T m_value;
    };

    template <typename Head, typename... Tail>
    class Tuple<Head, Tail...>
    {
    public:
        Tuple(Head head, Tail... tail)
          : m_head(head)
          , m_tail(tail...)
        {
        }
        ~Tuple() = default;

        Head head() const { return m_head; }

        Tuple<Tail...> tails() const { return m_tail; }

    private:
        Head           m_head;
        Tuple<Tail...> m_tail;
    };
}