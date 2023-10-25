#pragma once

namespace utils::func {
    template <typename T>
    class Predicate : public Function<bool(T)>
    {};
}