#include <arch/x86_64/types.h>
#include <arch/x86_64/interrupts.hpp>
#include <arch/x86_64/apic.hpp>
#include <common/logger.h>

IdtEntry idtEntries[IDT_ENTRY_COUNT];
InterruptData interrupts[256] = 
{
    [0] = { "Division Error", IntTypeFault, false },
    [1] = { "Debug", (IntTypeFault | IntTypeTrap), false },
    [2] = { "Non-maskable Interrupt", IntTypeInterrupt, false },
    [3] = { "Breakpoint", IntTypeTrap, false },
    [4] = { "Overflow", IntTypeTrap, false },
    [5] = { "Bound Range Exceeded", IntTypeFault, false },
    [6] = { "Invalid Opcode", IntTypeFault, false },
    [7] = { "Device Not Available", IntTypeFault, false },
    [8] = { "Double Fault", IntTypeAbort, true },
    [9] = { "Coprocessor Segment Overrun", IntTypeFault, false },
    [10] = { "Invalid Task State Segment", IntTypeFault, true },
    [11] = { "Segment Not Present", IntTypeFault, true },
    [12] = { "Stack-Segment Fault", IntTypeFault, true },
    [13] = { "General Protection Fault", IntTypeFault, true },
    [14] = { "Page Fault", IntTypeFault, true },
    [15] = { "Reserved", 0, false },
    [16] = { "x87 Floating-Point Exception", IntTypeFault, false },
    [17] = { "Alignment Check", IntTypeFault, true },
    [18] = { "Machine Check", IntTypeAbort, false },
    [19] = { "SIMD Floating-Point Exception", IntTypeFault, false },
    [20] = { "Virtualization Exception", IntTypeFault, false },
    [21] = { "Control Protection Exception", IntTypeFault, false },
    [22] = { "Reserved", 0, false },
    [23] = { "Reserved", 0, false },
    [24] = { "Reserved", 0, false },
    [25] = { "Reserved", 0, false },
    [26] = { "Reserved", 0, false },
    [27] = { "Reserved", 0, false },
    [28] = { "Hypervisor Injection Exception", IntTypeFault, false },
    [29] = { "VMM Communication Exception", IntTypeFault, true },
    [30] = { "Security Exception", IntTypeFault, true }
};

void UnhandledException(RegisterContext* context)
{
    Logger::getLogger("system").error("Unhandled Interrupt [%u]", context->intno);
    while (true)
        ;
}

extern "C" void dispatchInt(void* rsp)
{
    RegisterContext* context = reinterpret_cast<RegisterContext*>(rsp);
    InterruptData* data = &(interrupts[context->intno]);

    if (data->handler != nullptr) {
        data->handler(context);
    } else if (!(context->ss & 0x3)) {
        Logger::getLogger("system").printStackTrace();
        asm("hlt");
    }

    if (context->intno >= 0x20) {
        ApicDevice::getLocal().eoi();
    }

    return rsp;
}