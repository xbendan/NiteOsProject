#pragma once

namespace utils::function {
    template <typename T>
    class Predicate : public Function<bool(T)>
    {};
}