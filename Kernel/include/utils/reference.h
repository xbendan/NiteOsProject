namespace utils {
    template <typename T>
    struct RemoveReference
    {
        typedef T Type;
    };

    template <typename T>
    struct RemoveReference<T&>
    {
        typedef T Type;
    };

    template <typename T>
    struct RemoveReference<T&&>
    {
        typedef T Type;
    };

    template <typename T>
    class Reference
    {
    private:
        T*   ptr;
        int* ref_count;

    public:
        Reference()
          : ptr(nullptr)
          , ref_count(nullptr)
        {
        }

        Reference(T* p)
          : ptr(p)
          , ref_count(new int(1))
        {
        }

        Reference(const Reference<T>& other)
          : ptr(other.ptr)
          , ref_count(other.ref_count)
        {
            (*ref_count)++;
        }

        Reference<T>& operator=(const Reference<T>& other)
        {
            if (this != &other) {
                if (ptr != nullptr && --(*ref_count) == 0) {
                    delete ptr;
                    delete ref_count;
                }

                ptr       = other.ptr;
                ref_count = other.ref_count;
                (*ref_count)++;
            }

            return *this;
        }

        ~Reference()
        {
            if (ptr != nullptr && --(*ref_count) == 0) {
                delete ptr;
                delete ref_count;
            }
        }

        T& operator*() const { return *ptr; }

        T* operator->() const { return ptr; }

        operator bool() const { return ptr != nullptr; }
    };
}