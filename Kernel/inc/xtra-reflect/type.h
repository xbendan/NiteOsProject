#include <stdcxx/string.h>

namespace Xtra::Reflect {
    template <typename T>
    class Type
    {
    private:
        String<Utf8> _name;
    };

    template <typename T>
    static constexpr Type<T> typeOf()
    {
        return Type<T>();
    }
}