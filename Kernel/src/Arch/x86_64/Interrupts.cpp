#include <Drivers/Generic/APIC.h>

#include <Arch/x86_64/CPU.h>
#include <Arch/x86_64/Interrupts.h>
#include <System.h>

void UnhandlededException(InterruptData *data, RegisterContext *context)
{
    DisableInterrupts();
    System::Panic("Unhandled Expcetion! Name=%s", data->name);
    while (true)
        ;
}

InterruptData g_IntData[256] =
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

extern "C" void* DispatchInterrupts(void *rsp)
{
    RegisterContext *context = reinterpret_cast<RegisterContext *>(rsp);
    InterruptData *data = &g_IntData[context->intno];

    Halt();

    if (data->handler != nullptr)
    {
        data->handler(data, context);
    }
    else if (!(context->ss & 0x3))
    {
        Halt();
        System::Out("Interrupt=%u", context->intno);
        System::Out("RIP=%x, RSP=%x, RBP=%x", context->rip, context->rsp, context->rbp);
        System::Out("Stack Pointer=%x", *((uint64_t *) context->rsp));
        System::Out("RAX=%x, RBX=%x, RCX=%x", context->rax, context->rbx, context->rcx);
        System::Out("RDX=%x, RDI=%x", context->rdx, context->rdi);
    }

    if (context->intno >= 0x20)
        APIC::Local::EndOfInterrupt();

    return rsp;
}

bool RegisterIRQ(uint8_t intno, irqhandle_t handler)
{
    InterruptData *intData = &g_IntData[intno];
    if (handler == nullptr || intData->handler)
        return false;

    intData->handler = handler;
    return true;
}