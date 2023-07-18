template <typename T>
class Result<T> {
public:
    using ValueType = T;

    inline Result() = default;

    bool isEmpty() { return obj == nullptr; }
    T&   get() { return obj; }

private:
    T& obj;
};