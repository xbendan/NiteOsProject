#include <kern.h>
#include <Drivers/video.h>

void CallPanic(const char* text)
{
    Video::WriteText(text);
    for(;;) asm("hlt");
    // MakeTripleFault();
}