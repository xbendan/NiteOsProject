#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Kern {
    [[noreturn]] void panic(Std::String<Utf8> msg);
}