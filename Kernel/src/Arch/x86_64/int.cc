#include <Drivers/APIC.h>

#include <Arch/x86_64/pic.h>
#include <Arch/x86_64/cpu.h>
#include <Arch/x86_64/irq.h>
#include <kern.h>
#include <System.h>

void EnableInterrupts() { asm("sti"); }
void DisableInterrupts() { asm("cli"); }

static isr_t interruptHandlers[256];

extern "C" void* DispatchInterrupts(void *rsp)
{
    registers_t *context = reinterpret_cast<registers_t *>(rsp);

    System::Out("Interrupt: %u", context->intno);

    if (context->intno >= 32) {
        APIC::Local::EndOfInterrupt();
    }

    if (interruptHandlers[context->intno]) {
        interruptHandlers[context->intno](nullptr, context);
    } else if (!(context->ss & 0x3)) {

    }

    // RestoreInterrupts(context->intno);

    return rsp;
}

void RegisterInterruptHandler(int num, isr_t func) {
    if (interruptHandlers[num] != 0) {
        System::Out("The interrupt handler already exists.");
        return;
    }

    interruptHandlers[num] = func;
}