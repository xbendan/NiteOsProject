#include <mixins/std/type_traits.h>

template <typename _Tp>
    requires(!mixins::std::isTrivial<_Tp>::value)
class RefPtr
{
public:
    /* --- Constructors --- */
    RefPtr()
      : m_ptr(nullptr)
      , m_refCount(new unsigned(0))
    {
    }
    RefPtr(_Tp* ptr)
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

    /* --- Methods --- */

    unsigned refCount() const { return m_refCount ? *m_refCount : 0; }

    /* --- Operators --- */

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

    _Tp* operator->() const { return m_ptr; }

    _Tp& operator*() const { return *m_ptr; }

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

    _Tp*      m_ptr;
    unsigned* m_refCount;
};