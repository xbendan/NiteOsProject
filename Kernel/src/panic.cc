#include <kern.h>
#include <driver/video.h>

void CallPanic(const char* text)
{
    Video::WriteText(text);
    for(;;) asm("hlt");
    // MakeTripleFault();
}