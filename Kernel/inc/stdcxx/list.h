#include <stdcxx/iterator.h>
#include <stdcxx/types.h>

namespace Std {
    template <typename T>
    class List : IIterable<T>
    {
    public:
        virtual ~List() = default;

        virtual Void add(T const& value) = 0;
        virtual Void remove(UInt64 i)    = 0;
        virtual Void remove(T const& t)  = 0;
        virtual Void clear()             = 0;
        virtual T*   get(UInt64 i)       = 0;

        virtual T& operator[](UInt64 index) = 0;
    };
}
