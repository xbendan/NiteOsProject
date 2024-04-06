#include <mixins/std/type_traits.h>

template <typename _Tp>
    requires(!isTrivial<_Tp>::value)
class Ref
{
public:
    /* --- Constructors --- */
    Ref()
      : m_ptr(nullptr)
      , m_refCount(new unsigned(0))
    {
    }
    Ref(_Tp* ptr)
      : m_ptr(ptr)
    {
        if (m_ptr) {
            m_refCount = new unsigned(1);
        } else {
            m_refCount = nullptr;
        }
    }
    Ref(const Ref& other)
      : m_ptr(other.m_ptr)
      , m_refCount(other.m_refCount)
    {
        if (m_refCount) {
            (*m_refCount)++;
        }
    }

    ~Ref() { release(); }

    /* --- Methods --- */

    unsigned refCount() const { return m_refCount ? *m_refCount : 0; }

    /* --- Operators --- */

    Ref& operator=(const Ref& other)
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

    _Tp* m_ptr;
    unsigned* m_refCount;
};