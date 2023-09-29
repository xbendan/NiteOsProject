#pragma once

#include <utils/collection.h>
#include <utils/functions/predicate.h>
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

        bool allMatch(utils::function::Predicate<T> predicate);

        bool anyMatch(utils::function::Predicate<T> predicate);

        void forEach(utils::function::Consumer<T> consumer);

        Stream<T>& limit(unsigned size);

        Stream<T>& skip(unsigned size);
    };
}