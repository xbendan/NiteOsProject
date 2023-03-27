#include <Proc/Scheduler.h>
#include <Proc/Process.h>
#include <Device/Device.h>
#include <Drivers/Input/PS2LegacyController.h>
#include <Drivers/video.h>
#include <Input/Console.h>
#include <Mem/Page.h>
#include <Timer.h>

#include <libkern/printf.h>
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

Timer *g_Timers[5];
Console *g_Console;
TimerType Timer::g_TypePreferrence;

void Sleep(long milliseconds)
{
    Timer *timer = g_Timers[Timer::g_TypePreferrence];
    if (timer != nullptr)
    {
        timer->Sleep(milliseconds);
    }
}

using namespace Task;

[[noreturn]] void KernelInit()
{
    (new Input::PS2LegacyController())->Register();
    g_Scheduler = new Scheduler();
    g_Console = new Console();
    
    for (;;) asm("hlt");
}
