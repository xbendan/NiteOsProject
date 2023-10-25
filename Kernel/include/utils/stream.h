#pragma once

#include <utils/collection.h>
#include <utils/func/predicate.h>
#include <utils/optional.h>

namespace utils {
    template <typename T>
    class Stream
    {
    public:
        Stream();
        Stream(Collection<T>& c);
        Stream(unsigned size);
        ~Stream();

        Stream<T>& filter(Predicate<T> predicate);

        template <typename N>
        Stream<N>& map(N (*function_t)(T&));

        Optional<T> findFirst();

        bool allMatch(utils::func::Predicate<T> predicate);

        bool anyMatch(utils::func::Predicate<T> predicate);

        void forEach(utils::func::Consumer<T> consumer);

        Stream<T>& limit(unsigned size);

        Stream<T>& skip(unsigned size);
    };
}