#include <Drivers/APIC.h>

#include <Arch/x86_64/pic.h>
#include <Arch/x86_64/CPU.h>
#include <Arch/x86_64/Interrupts.h>
#include <System.h>

static irqhandle_t interruptHandlers[256];

void UnhandlededException(InterruptData *data, RegisterContext *context)
{
    
}

InterruptData g_IntData[256] =
{
    [0] = { "Division Error", IntTypeFault, false},
    [1] = { "Debug", (IntTypeFault | IntTypeTrap), false},
    [2] = { "Non-maskable Interrupt", IntTypeInterrupt, false},
    [3] = { "Breakpoint", IntTypeTrap, false},
    [4] = { "Overflow", IntTypeTrap, false},
    [5] = { "Bound Range Exceeded", IntTypeFault, false},
    [6] = { "Invalid Opcode", IntTypeFault, false},
    [7] = { "Device Not Available", IntTypeFault, false},
    [8] = { "Double Fault", IntTypeAbort, true},
    [9] = { "Coprocessor Segment Overrun", IntTypeFault, false},
    [10] = { "Invalid Task State Segment", IntTypeFault, true},
    [11] = { "Segment Not Present", IntTypeFault, true},
    [12] = { "Stack-Segment Fault", IntTypeFault, true},
    [13] = { "General Protection Fault", IntTypeFault, true},
    [14] = { "Page Fault", IntTypeFault, true},
    [15] = { "Reserved", 0, false},
    [16] = { "x87 Floating-Point Exception", IntTypeFault, false},
    [17] = { "Alignment Check", IntTypeFault, true},
    [18] = { "Machine Check", IntTypeAbort, false},
    [19] = { "SIMD Floating-Point Exception", IntTypeFault, false},
    [20] = { "Virtualization Exception", IntTypeFault, false},
    [21] = { "Control Protection Exception", IntTypeFault, false},
    [22] = { "Reserved", 0, false},
    [23] = { "Reserved", 0, false},
    [24] = { "Reserved", 0, false},
    [25] = { "Reserved", 0, false},
    [26] = { "Reserved", 0, false},
    [27] = { "Reserved", 0, false},
    [28] = { "Hypervisor Injection Exception", IntTypeFault, false},
    [29] = { "VMM Communication Exception", IntTypeFault, true},
    [30] = { "Security Exception", IntTypeFault, true},
};

extern "C" void* DispatchInterrupts(void *rsp)
{
    RegisterContext *context = reinterpret_cast<RegisterContext *>(rsp);

    if (interruptHandlers[context->intno]) {
        interruptHandlers[context->intno](nullptr, context);
    } else if (!(context->ss & 0x3)) {

    }

    if (context->intno >= 0x20)
        APIC::Local::EndOfInterrupt();

    return rsp;
}

void RegisterInterruptHandler(int num, irqhandle_t func) {
    if (interruptHandlers[num] != 0) {
        System::Out("The interrupt handler already exists.");
        return;
    }

    interruptHandlers[num] = func;
}