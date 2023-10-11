namespace utils {
    template <typename T>
    class Queue : LinkedList<T>
    {
    public:
        Queue();
        ~Queue();

        T& next();
    };
}