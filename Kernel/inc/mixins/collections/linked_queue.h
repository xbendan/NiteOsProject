#include <mixins/collections/linked_list.h>

template <typename _Tp>
    requires(!Std::isTrivial<_Tp>::value)
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

    _Tp* enqueue(_Tp const& value) { return LinkedList<_Tp>::pushBack(value); }

    _Tp* dequeue()
    {
        auto elem = LinkedList<_Tp>::get(0);
        LinkedList<_Tp>::remove(0);
        return elem;
    }

    _Tp* peek() { return LinkedList<_Tp>::get(0); }
};