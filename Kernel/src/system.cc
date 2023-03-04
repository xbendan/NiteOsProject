#include <libkern/printf.h>
#include <driver/video.h>
#include <macros>

namespace System
{
    void Out(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        Video::Newline();
    }

    [[noreturn]] void Panic(const char *fmt, ...) {
        Video::ClearScreen();
        
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        asm("cli; hlt;");
    }

    void EntryPoint() {
        
    }
} // namespace System
