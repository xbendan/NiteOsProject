#include <Arch/x86_64/IDT.h>
#include <Arch/x86_64/CPU.h>

extern uintptr_t isr_tables[];
static struct InterruptDescTableEntry g_IdtEntryList[IDT_ENTRY_COUNT];
struct InterruptDescTablePointer g_Idtr = {
    .size = sizeof(struct InterruptDescTableEntry) * IDT_ENTRY_COUNT,
    .base = (uint64_t) &g_IdtEntryList[0]
};

namespace IDT
{
    void Initialize()
    {
        for (uint8_t num = 0; num < 48; num++)
            g_IdtEntryList[num] = (struct InterruptDescTableEntry){
                .base_low = (isr_tables[num] & 0xffff),
                .selector = 0x08,
                .ist = 0x0,
                .flags = IDT_FLAGS_INTGATE,
                .base_med = ((isr_tables[num] >> 16) & 0xffff),
                .base_high = ((isr_tables[num] >> 32) & 0xffffffff)
            };
            

        g_IdtEntryList[127] = (struct InterruptDescTableEntry){
            .base_low = (isr_tables[32] & 0xffff),
            .selector = 0x08,
            .ist = 0x0,
            .flags = IDT_FLAGS_INTGATE,
            .base_med = ((isr_tables[32] >> 16) & 0xffff),
            .base_high = ((isr_tables[32] >> 32) & 0xffffffff)
        };
        g_IdtEntryList[128] = (struct InterruptDescTableEntry){
            .base_low = (isr_tables[33] & 0xffff),
            .selector = 0x08,
            .ist = 0x0,
            .flags = IDT_FLAGS_INTGATE | IDT_FLAGS_USER,
            .base_med = ((isr_tables[33] >> 16) & 0xffff),
            .base_high = ((isr_tables[33] >> 32) & 0xffffffff)
        };

        asmw_flush_idt((uint64_t) &g_Idtr);
    }
} // namespace IDT