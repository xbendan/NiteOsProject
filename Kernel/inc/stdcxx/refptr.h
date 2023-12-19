#include <stdcxx/types.h>

template <typename T>
class RefPtr
{
public:
    RefPtr(T* ptr)
      : m_ptr(ptr)
    {
        if (m_ptr) {
            m_refCount = new unsigned(1);
        } else {
            m_refCount = nullptr;
        }
    }

    RefPtr(const RefPtr& other)
      : m_ptr(other.m_ptr)
      , m_refCount(other.m_refCount)
    {
        if (m_refCount) {
            (*m_refCount)++;
        }
    }

    ~RefPtr() { release(); }

    RefPtr& operator=(const RefPtr& other)
    {
        if (this != &other) {
            release();
            m_ptr      = other.m_ptr;
            m_refCount = other.m_refCount;
            if (m_refCount) {
                (*m_refCount)++;
            }
        }
        return *this;
    }

    T* operator->() const { return m_ptr; }

    T& operator*() const { return *m_ptr; }

private:
    void release()
    {
        if (m_refCount) {
            (*m_refCount)--;
            if (*m_refCount == 0) {
                delete m_ptr;
                delete m_refCount;
            }
        }
    }

    T*   m_ptr;
    int* m_refCount;
};
