#include <stdcxx/string.h>
#include <stdcxx/types.h>

namespace Kern {
    [[noreturn]] void callPanic(Std::String<Utf8> msg);
}