#include <Arch/x86_64/pic.h>
#include <Arch/x86_64/cpu.h>
#include <kern.h>
#include <drv/video.h>

void EnableInterrupts() { asm("sti"); }
void DisableInterrupts() { asm("cli"); }

extern "C" void* DispatchInterrupts(void *rsp)
{
    registers_t *context = reinterpret_cast<registers_t *>(rsp);

    if (context->intno < 32)
    {
        Video::WriteText("INT");
    } else if (context->intno < 48) {

    }

    RestoreInterrupts(context->intno);

    return rsp;
}