#include <siberix/panic.h>

extern "C" void
__cxa_pure_virtual()
{
    Kern::callPanic("Pure virtual function called");
}

namespace __cxxabiv1 {

    __extension__ typedef int __guard __attribute__((mode(__DI__)));

    extern "C" int __cxa_guard_acquire(__guard* g)
    {
        return !*(char*)(g);
    }

    extern "C" void __cxa_guard_release(__guard* g)
    {
        *(char*)g = 1;
    }

    extern "C" void __cxa_guard_abort(__guard*) {}

}