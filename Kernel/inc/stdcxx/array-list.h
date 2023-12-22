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
        ArrayList(uint32_t capacity)
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

        void add(T const& value) {}

        virtual void remove(uint64_t i)   = 0;
        virtual void remove(T const& t) = 0;
        virtual void clear()            = 0;
        virtual T*   get(uint64_t i)      = 0;

        /* --- Operators --- */
        T& operator[](uint64_t index)
        {
            if (index >= _length) {
                // TODO: Throw exception
            }
            return _buffer[index];
        }

    private:
        T*     _buffer;
        uint64_t _capacity;
        uint64_t _length;
    };
}