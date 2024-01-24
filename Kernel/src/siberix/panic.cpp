#include <siberix/panic.h>

namespace Kern {
    [[noreturn]] void callPanic(Std::String<Utf8> msg)
    {
        asm volatile("cli");
        while (true) {
            asm volatile("hlt");
        }
    }
}