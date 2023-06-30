#include <macros>

namespace System
{
    void Out(const char* fmt, ...);

    void Reboot();
    void Shutdown();
    [[noreturn]] void Panic(const char *msg, ...);
} // namespace System

void Halt();

[[noreturn]] void KernelInit();
