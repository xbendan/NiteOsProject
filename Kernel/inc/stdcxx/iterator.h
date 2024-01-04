namespace Std {
    template <typename T>
    class Iterator
    {
    public:
        virtual Iterator& operator++() = 0;
        virtual T&        operator*()  = 0;
    };
}