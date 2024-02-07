#include <stdcxx/linked_list.h>

namespace Std {
    template <typename T>
    class Queue : protected LinkedList<T>
    {
    public:
        Queue()
          : _list()
        {
        }

        Queue(Queue<T> const& other)
          : _list(other._list)
        {
        }

        ~Queue() = default;

        void enqueue(T const& value) { _list.add(value); }

        T* dequeue()
        {
            auto elem = get(0);
            remove(0);
            return elem;
        }

        T* peek() { return _list.get(0); }
    };
}