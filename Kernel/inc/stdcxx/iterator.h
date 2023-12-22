#include <stdcxx/array.h>

namespace Std {
    template <typename T>
    class Iterator
    {
    public:
        Iterator(uint64_t length)
          : _list(new T*[length])
        {
        }

        bool hasNext() { return _index < _list.length(); }

        T* next()
        {
            if (!hasNext())
                return nullptr;

            return _list[_index++];
        }

    private:
        T**    _list;
        uint64_t _index;
    };

    template <typename T>
    class IIterable
    {
        virtual Iterator<T>& iterator() = 0;
    };
}