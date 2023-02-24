#include <kern.h>
#include <drv/video.h>

void CallPanic(const char* text)
{
    Video::WriteText(text);
    for(;;) asm("hlt");
    // MakeTripleFault();
}