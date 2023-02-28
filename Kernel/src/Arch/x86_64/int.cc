#include <Arch/x86_64/pic.h>
#include <Arch/x86_64/apic.h>
#include <Arch/x86_64/cpu.h>
#include <Arch/x86_64/irq.h>
#include <kern.h>
#include <drv/video.h>

void EnableInterrupts() { asm("sti"); }
void DisableInterrupts() { asm("cli"); }

static isr_t interruptHandlers[256];

extern "C" void* DispatchInterrupts(void *rsp)
{
    registers_t *context = reinterpret_cast<registers_t *>(rsp);

    if (context->intno >= 32) {
        APIC::Local::EndOfInterrupt();
        // Video::WriteChar('X');
    }

    if (context->intno < 14) {
        Video::WriteChar('L');
        asm("cli");
        asm("hlt");
    }

    if (interruptHandlers[context->intno]) {
        interruptHandlers[context->intno](nullptr, context);
    } else if (!(context->ss & 0x3)) {

    }

    RestoreInterrupts(context->intno);

    return rsp;
}

void RegisterInterruptHandler(int num, isr_t func) {
    if (interruptHandlers[num] != 0) {
        Video::WriteText("The interrupt handler already exists.");
        return;
    }

    interruptHandlers[num] = func;
}