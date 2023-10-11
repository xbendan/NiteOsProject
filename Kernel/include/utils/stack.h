#include <utils/array_list.h>

namespace utils {
    template <typename T>
    class Stack : private ArrayList<T>
    {
    public:
        Stack()
          : ArrayList(8)
        {
        }
        ~Stack() = default;

        void push(T obj) { add(obj); }

        Optional<T> pop() { return get(size()); }
    };
}