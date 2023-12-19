#include <stdcxx/array.h>

namespace Std {
    template <typename T>
    class Iterator
    {
    public:
        Iterator(UInt64 length)
          : _list(new T*[length])
        {
        }

        Boolean hasNext() { return _index < _list.length(); }

        T* next()
        {
            if (!hasNext())
                return nullptr;

            return _list[_index++];
        }

    private:
        T**    _list;
        UInt64 _index;
    };

    template <typename T>
    class IIterable
    {
        virtual Iterator<T>& iterator() = 0;
    };
}