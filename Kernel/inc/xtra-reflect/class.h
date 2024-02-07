#include <stdcxx/string.h>
#include <stdcxx/type-traits.h>

template <class T>
    requires Std::isClass<T>::value
class Class
{
public:
    T* newInst() { return new T(); }
    T* newInstStatic()
    {
        static T instance = T();
        return &instance;
    }
    Std::String<Utf8> getName();
};

template <class T>
    requires Std::isClass<T>::value
Class<T>
classOf()
{
    return Class<T>();
}