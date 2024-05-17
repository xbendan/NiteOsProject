#include <mixins/meta/opt.h>
#include <mixins/std/type_traits.h>
#include <mixins/utils/linked_list.h>

namespace mixins::utils {
    template <typename _Tp>
        requires(!std::isTrivial<_Tp>::value)
    class Queue : protected LinkedList<_Tp>
    {
    public:
        Queue()
          : LinkedList<_Tp>()
        {
        }

        Queue(Queue const& other)
          : LinkedList<_Tp>(other)
        {
        }

        ~Queue() = default;

        _Tp* enqueue(_Tp const& value)
        {
            return LinkedList<_Tp>::pushBack(value);
        }

        _Tp* dequeue() { return LinkedList<_Tp>::popFront(); }

        _Tp* peek() { return LinkedList<_Tp>::get(0); }
    };
}