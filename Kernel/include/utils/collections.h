template<typename T>
class Stream;

template<typename T>
class Collection
{
public:
    Collection()  = default;
    ~Collection() = default;

    virtual int        size()                            = 0;
    virtual T&         get(unsigned index)               = 0;
    virtual int        indexOf(T& objRef)                = 0;
    virtual int        lastIndexOf(T& objRef)            = 0;
    virtual void       insert(T& objRef, unsigned index) = 0;
    virtual void       add(T& objRef)                    = 0;
    virtual bool       remove(unsigned index)            = 0;
    virtual bool       remove(T& objRef)                 = 0;
    virtual Stream<T>& stream()                          = 0;
};