struct list_head {
    listhead *next, *prev;
} listhead_t;

template <typename T>
struct list_node : list_head {
    T& obj;

    listnode(T obj)
        : obj(obj) { }
};

template <typename T>
class linked_list {
private:
    list_node<T> *front, *back;
    uint32_t count;

public:
    linked_list() {
        front = nullptr, back = nullptr;
        count = 0;
    }
}