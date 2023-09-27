template<typename T>
class Supplier
{
public:
    Supplier() = default;
    Supplier(T const* objPtr)
      : m_objPtr(objPtr)
    {
    }
    ~Supplier() {}

    bool isEmpty() { return m_objPtr == nullptr; }
    bool isPresent() { return m_objPtr != nullptr; }
    T*   get() { return m_objPtr; }
    T*   orElse(T* fallback) { return isPresent() ? m_objPtr : fallback; }
    void ifPresent(void (*consume_t)(T*));

    bool operator==(const Supplier<T>& val) { return m_objPtr == val.m_objPtr; }

private:
    T* m_objPtr;
};