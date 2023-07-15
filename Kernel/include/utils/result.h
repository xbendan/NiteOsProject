template <typename T>
class Result {
public:
    bool isEmpty() { return obj == nullptr; }
    T&   get() { return obj; }

private:
    T& obj;
};