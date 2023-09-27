template<typename T>
class RefPtr
{
public:
    inline RefPtr<T>& operator=(const RefPtr<T>& ptr)
    {
        obj      = ptr.obj;
        refCount = ptr.refCount;

        if (obj && refCount) {
        }

        return *this;
    }
    inline bool operator==(const T* p) { return (obj == p); }
    inline bool operator!=(const T* p) { return (obj != p); }
    inline T&   operator*() const { return *(obj); }
    inline T*   operator->() const { return obj; }

protected:
    unsigned* refCount = nullptr;
    T*        obj      = nullptr;
};