#include <Proc/Scheduler.h>
#include <Proc/Process.h>
#include <libkern/printf.h>
#include <Drivers/video.h>
#include <Mem/Page.h>
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
        //Video::ClearScreen();
        
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);

        for (;;) asm("cli; hlt;");
    }
} // namespace System

using namespace Task;

[[noreturn]] void KernelInit()
{
    g_Scheduler.m_IdleProcess = g_Scheduler.CreateIdleProcess();
    System::Out("%u", sizeof(Scheduler));
    for (;;) asm("hlt");
}
