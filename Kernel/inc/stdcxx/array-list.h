#include <stdcxx/type-traits.h>
#include <stdcxx/list.h>

#define DEFAULT_ARRAY_LIST_CAPACITY 8
#define DEFAULT_ARRAY_LIST_GROWTH_FACTOR 2

namespace Std {
    template <typename T>
        requires IsIntegral<T>
    class ArrayList : public List<T>
    {
    public:
        /* --- Constructors --- */
        ArrayList()
          : ArrayList(DEFAULT_ARRAY_LIST_CAPACITY)
        {
        }
        ArrayList(UInt32 capacity)
          : _buffer(new T[capacity])
          , _capacity(capacity)
          , _length(0)
        {
        }
        ~ArrayList()
        {
            delete[] _buffer;
            _length = 0;
        }

        /* --- Methods --- */

        Void add(T const& value) {}

        virtual Void remove(UInt64 i)   = 0;
        virtual Void remove(T const& t) = 0;
        virtual Void clear()            = 0;
        virtual T*   get(UInt64 i)      = 0;

        /* --- Operators --- */
        T& operator[](UInt64 index)
        {
            if (index >= _length) {
                // TODO: Throw exception
            }
            return _buffer[index];
        }

    private:
        T*     _buffer;
        UInt64 _capacity;
        UInt64 _length;
    };
}