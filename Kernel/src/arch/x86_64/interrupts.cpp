#include <arch/x86_64/apic.h>
#include <arch/x86_64/interrupts.h>
#include <arch/x86_64/iopt.h>
#include <arch/x86_64/types.h>
#include <common/logger.h>
#include <siberix/display/types/vga.h>

IdtEntry idtEntryList[IDT_ENTRY_COUNT];

void UnhandledException(RegisterContext* context) {
    Logger::getAnonymousLogger().error("Unhandled Interrupt [%u]", context->intno);
    while (true)
        ;
}

void GeneralProtectionFault(RegisterContext* context) {
    char buf[64];
    u64  addr;
    asm volatile("mov %%cr2, %[addr]" : [addr] "=r"(addr));

    // _vga.drawText({ -1, -1 }, "General Protection Fault", Color(VgaTextColor::Red));
    // _vga.drawText({ -1, -1 }, utoa(context->err, buf, 16), Color(VgaTextColor::Red));
    // _vga.drawText({ -1, -1 }, utoa(context->rax, buf, 16), Color(VgaTextColor::Red));

    for (;;) asm("cli; hlt");
}

void PageFault(RegisterContext* context) {
    Logger::getAnonymousLogger().info("Page Fault [%u]", context->intno);
    Logger::getAnonymousLogger().info("Error Code: %u", context->err);
    Logger::getAnonymousLogger().info("RAX: %x", context->rax);

    for (;;) asm("cli; hlt");
}

InterruptData interrupts[256] = {
    [0]  = {"Division Error",                  IntTypeFault,                 false},
    [1]  = { "Debug",                          (IntTypeFault | IntTypeTrap), false},
    [2]  = { "Non-maskable Interrupt",         IntTypeInterrupt,             false},
    [3]  = { "Breakpoint",                     IntTypeTrap,                  false},
    [4]  = { "Overflow",                       IntTypeTrap,                  false},
    [5]  = { "Bound Range Exceeded",           IntTypeFault,                 false},
    [6]  = { "Invalid Opcode",                 IntTypeFault,                 false},
    [7]  = { "Device Not Available",           IntTypeFault,                 false},
    [8]  = { "Double Fault",                   IntTypeAbort,                 true },
    [9]  = { "Coprocessor Segment Overrun",    IntTypeFault,                 false},
    [10] = { "Invalid Task State Segment",     IntTypeFault,                 true },
    [11] = { "Segment Not Present",            IntTypeFault,                 true },
    [12] = { "Stack-Segment Fault",            IntTypeFault,                 true },
    [13] = { "General Protection Fault",       IntTypeFault,                 true, GeneralProtectionFault, },
    [14] = { "Page Fault",                     IntTypeFault,                 true, PageFault, },
    [15] = { "Reserved",                       0,                            false},
    [16] = { "x87 Floating-Point Exception",   IntTypeFault,                 false},
    [17] = { "Alignment Check",                IntTypeFault,                 true },
    [18] = { "Machine Check",                  IntTypeAbort,                 false},
    [19] = { "SIMD Floating-Point Exception",  IntTypeFault,                 false},
    [20] = { "Virtualization Exception",       IntTypeFault,                 false},
    [21] = { "Control Protection Exception",   IntTypeFault,                 false},
    [22] = { "Reserved",                       0,                            false},
    [23] = { "Reserved",                       0,                            false},
    [24] = { "Reserved",                       0,                            false},
    [25] = { "Reserved",                       0,                            false},
    [26] = { "Reserved",                       0,                            false},
    [27] = { "Reserved",                       0,                            false},
    [28] = { "Hypervisor Injection Exception", IntTypeFault,                 false},
    [29] = { "VMM Communication Exception",    IntTypeFault,                 true },
    [30] = { "Security Exception",             IntTypeFault,                 true }
};

extern "C" void* fDispatchInterrupts(RegisterContext* context) {
    InterruptData* data = &(interrupts[context->intno]);

    if (data->handler != nullptr) {
        data->handler(context);
    } else if (!(context->ss & 0x3)) {
        Logger::getAnonymousLogger().printStackTrace();
    }

    if (context->intno >= 0x20) {
        _apic->getInterface().eoi();
    }

    return (void*)context;
}