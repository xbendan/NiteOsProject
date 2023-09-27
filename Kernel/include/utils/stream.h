#include <utils/collections.h>
#include <utils/supplier.h>

template<typename T>
class Stream
{
public:
    Stream();
    Stream(Collection<T>& c);
    Stream(unsigned size);
    ~Stream();

    Stream<T>& filter(bool (*predicate_t)(T&));

    template<typename N>
    Stream<N>& map(N (*function_t)(T&));

    Supplier<T> findFirst();

    bool allMatch(bool (*predicate_t)(T&));

    bool anyMatch(bool (*predicate_t)(T&));

    void forEach(void (*consume_t)(T&));

    Stream<T>& limit(unsigned size);

    Stream<T>& skip(unsigned size);
};